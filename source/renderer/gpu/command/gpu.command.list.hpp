#pragma once

#include <renderer/gpu/command/gpu.command.hpp>
#include <renderer/gpu/core/gpu.limits.hpp>

struct GpuCommandList
{
    GpuCommand items[GpuLimits::MaxCommands] = {};
    u32 count = 0;

    bool push(const GpuCommand& command)
    {
        if (count >= GpuLimits::MaxCommands)
            return false_value;

        items[count++] = command;
        return true_value;
    }

    void reset()
    {
        count = 0;
    }

    bool empty() const
    {
        return count == 0;
    }
};
