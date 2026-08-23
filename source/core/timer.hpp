#pragma once

#include <core/types.hpp>

// TODO: Conflict
// #include <vendor/libc/stdlib.hpp>
// #include <vendor/libc/string.hpp>
#include <stdlib.h>
#include <string.h>

struct TimeSpec
{
    int_64 tv_sec;
    int_64 tv_nsec;
};

// MOVE
extern "C" int clock_gettime(int, TimeSpec *);
static constexpr int MONOTONIC = 1;

static INLINE uint_64 alloc_time_ns()
{
#if PLATFORM_LINUX
    TimeSpec ts{};
    clock_gettime(MONOTONIC, &ts);

    return static_cast< uint_64 >(ts.tv_sec) * 1000000000ULL + static_cast< uint_64 >(ts.tv_nsec);

#elif PLATFORM_WINDOWS
    // TODO:
    // return GetTickCount64() * 1000000ULL;
    return 0;
#endif
}

using TimerCallback = void (*)(void *user_data);

struct ScheduledTask
{
    TimerCallback callback;
    void *user_data;
    uint_64 time_remaining_us;
    uint_64 interval_us;
    bool repeat;
    bool active;
};

#if PLATFORM_LINUX
    extern "C" int nanosleep(const TimeSpec *, TimeSpec *);
#elif PLATFORM_WINDOWS
    extern "C" void Sleep(unsigned long);
#endif

namespace Timer
{
    namespace Get
    {
        static INLINE uint_64 microseconds()
        {
            return alloc_time_ns() / 1000ULL;
        }
    }

    static INLINE void sleep_microseconds( const uint_64 microseconds)
    {
#if PLATFORM_LINUX
        TimeSpec req{};
        req.tv_sec = static_cast<int_64>(microseconds / 1000000ULL);
        req.tv_nsec = static_cast<int_64>((microseconds % 1000000ULL) * 1000ULL);
        nanosleep(&req, nullptr);
#elif PLATFORM_WINDOWS
        Sleep(static_cast<unsigned long>(microseconds / 1000ULL));
#endif
    }
}

class TimerScheduler
{
private:
    static constexpr unsigned_size MAX_TASKS = 16;
    inline static ScheduledTask m_tasks[MAX_TASKS] = {};

public:

    static void schedule( const uint_64 delay_us, TimerCallback callback, void *user_data = nullptr, bool repeat = false_value )
    {
        for (unsigned_size i = 0; i < MAX_TASKS; ++i)
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

    static uint_64 get_time()
    {
        return Timer::Get::microseconds();
    }

    // Atualiza todos os timers ativos (chamado uma vez por frame no main loop)
    static void update( const uint_64 delta_us)
    {
        for (unsigned_size i = 0; i < MAX_TASKS; ++i)
        {
            if (!m_tasks[i].active)
                continue;

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