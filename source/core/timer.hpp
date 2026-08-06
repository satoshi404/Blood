#pragma once

#include <core/types.hpp>
#include <core/memory.hpp> // usa alloc_time_ns() como base de clock monotônico

// Assinatura da função a ser chamada: recebe um ponteiro genérico opcional
using TimerCallback = void(*)(void* user_data);

struct ScheduledTask
{
    TimerCallback callback;
    void* user_data;
    u64 time_remaining_us;
    u64 interval_us; // usado quando repeat == true, pra reiniciar a contagem
    bool repeat;
    bool active;
};

#if PLATFORM_LINUX
// TimeSpec já é declarado em core/memory.hpp; só precisamos do nanosleep aqui.
extern "C" int nanosleep( const TimeSpec*, TimeSpec* );
#elif PLATFORM_WINDOWS
extern "C" void Sleep( unsigned long );
#endif

// Namespace utilitário para ler o relógio monotônico em microssegundos
// e fazer frame pacing (limitar o FPS).
// (main.cpp chama Timer::Get::microseconds() a cada frame pra calcular o delta)
namespace Timer
{
    namespace Get
    {
        static INLINE u64 microseconds()
        {
            return alloc_time_ns() / 1000ULL;
        }
    }

    // Dorme a thread pelo tempo pedido (em microssegundos).
    // Usado pra cravar um teto de FPS (ex.: 60) sem gastar 100% da CPU.
    static INLINE void sleep_microseconds( u64 microseconds )
    {
#if PLATFORM_LINUX
        TimeSpec req{};
        req.tv_sec  = static_cast<i64>( microseconds / 1000000ULL );
        req.tv_nsec = static_cast<i64>( ( microseconds % 1000000ULL ) * 1000ULL );
        nanosleep( &req, nullptr );
#elif PLATFORM_WINDOWS
        Sleep( static_cast<unsigned long>( microseconds / 1000ULL ) );
#endif
    }
}

class TimerScheduler
{
private:
    // Limite máximo de timers simultâneos (ajuste conforme necessário)
    static constexpr usize MAX_TASKS = 16;
    inline static ScheduledTask m_tasks[MAX_TASKS] = {};

public:
    // Adiciona uma nova tarefa na fila estática.
    // repeat = true faz o callback disparar de novo a cada `delay_us`, indefinidamente.
    static void schedule(u64 delay_us, TimerCallback callback, void* user_data = nullptr, bool repeat = false)
    {
        for (usize i = 0; i < MAX_TASKS; ++i)
        {
            if (!m_tasks[i].active)
            {
                m_tasks[i].callback = callback;
                m_tasks[i].user_data = user_data;
                m_tasks[i].time_remaining_us = delay_us;
                m_tasks[i].interval_us = delay_us;
                m_tasks[i].repeat = repeat;
                m_tasks[i].active = true;
                return;
            }
        }
        // TODO: Assert ou log se estourar o MAX_TASKS
    }

    static u64 get_time()
    {
        return Timer::Get::microseconds();
    }

    // Atualiza todos os timers ativos (chamado uma vez por frame no main loop)
    static void update(u64 delta_us)
    {
        for (usize i = 0; i < MAX_TASKS; ++i)
        {
            if (!m_tasks[i].active) continue;

            if (delta_us >= m_tasks[i].time_remaining_us)
            {
                if (m_tasks[i].repeat)
                {
                    // Repetitivo: dispara e reinicia a contagem pro próximo ciclo
                    m_tasks[i].callback(m_tasks[i].user_data);
                    m_tasks[i].time_remaining_us = m_tasks[i].interval_us;
                }
                else
                {
                    // Tempo esgotou: executa o callback e desativa
                    m_tasks[i].active = false;
                    m_tasks[i].callback(m_tasks[i].user_data);
                }
            }
            else
            {
                m_tasks[i].time_remaining_us -= delta_us;
            }
        }
    }
};