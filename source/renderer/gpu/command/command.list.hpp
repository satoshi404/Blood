#pragma once

#include <renderer/gpu/command/command.hpp>
#include <renderer/gpu/core/limits.hpp>

struct CommandList
{
    Command items[ Limits::Max_Commands ] = {};
    uint_32 count = 0;

    bool push( const Command& command )
    {
        if ( count >= Limits::Max_Commands ) return false_value;

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
