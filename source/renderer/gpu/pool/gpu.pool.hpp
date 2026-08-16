#pragma once

#include <core/types.hpp>

template<typename T, typename Handle, usize Capacity>
struct GpuPool
{
    struct Slot
    {
        T value;
        u32 generation = 1;
        bool occupied = false;
    };

    Slot slots[Capacity];
};