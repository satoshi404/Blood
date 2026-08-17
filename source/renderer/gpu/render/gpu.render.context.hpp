#pragma once

#include <renderer/gpu/render/gpu.render.pass.hpp>

struct GpuRenderContext
{
	GpuRenderPass* current_pass = nullptr;
	GpuRenderQueue* current_queue = nullptr;

	GpuViewport viewport = {};
	GpuRenderState render_state = {};
	GpuMaterialHandle bound_material = {};
	GpuShaderHandle bound_shader = {};
	GpuMeshHandle bound_mesh = {};

	u32 draw_calls = 0;
	u32 vertices = 0;

	void reset()
	{
		current_pass = nullptr;
		current_queue = nullptr;
		bound_material = {};
		bound_shader = {};
		bound_mesh = {};
		draw_calls  = 0;
		vertices = 0;
	}
};