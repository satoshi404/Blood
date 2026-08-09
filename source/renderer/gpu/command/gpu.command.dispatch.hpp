#pragma once

#include <renderer/gpu/command/gpu.command.hpp>

namespace GpuCommandDispatcher
{
    void execute(const GpuCommand& command);
    void execute(const GpuCommand* commands, u32 count);
}
