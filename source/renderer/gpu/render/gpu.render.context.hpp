#pragma once

#include <renderer/gpu/render/gpu.render.pass.hpp>

struct GpuRenderContext
{
	RenderPass* current_pass = nullptr;
	RenderQueue* current_queue = nullptr;

	Viewport viewport = {};
	RenderState render_state = {};
	MaterialHandle bound_material = {};
	ShaderHandle bound_shader = {};
	MeshHandle bound_mesh = {};

	uint_32 draw_calls = 0;
	uint_32 vertices = 0;

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