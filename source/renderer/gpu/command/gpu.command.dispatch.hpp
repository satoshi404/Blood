#pragma once

#include <renderer/gpu/command/gpu.command.hpp>

namespace CommandDispatcher
{
    void execute(const Command& command);
    void execute(const Command* commands, uint_32 count);
}
