#pragma once

#include <core/types.hpp>
#include <renderer/gpu/resource/shader.hpp>

namespace ShaderPool
{
    bool init();
    void shutdown();

    ShaderHandle create( const Shader& shader );
    bool         destroy( ShaderHandle handle );
    Shader*      get( ShaderHandle handle );
    const Shader* get_const( ShaderHandle handle );
    bool         exists( ShaderHandle handle );

	extern uint_32 size();
	extern uint_32 capacity();
}