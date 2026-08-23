#pragma once

#include <renderer/gpu/command/gpu.command.hpp>
#include <renderer/gpu/core/limits.hpp>

struct CommandList
{
    Command items[ Limits::MaxCommands ] = {};
    u32 count = 0;

    bool push( const Command& command )
    {
        if ( count >= Limits::MaxCommands ) return false_value;

        items[count++] = command
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
