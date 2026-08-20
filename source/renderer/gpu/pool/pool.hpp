#pragma once

#include <core/types.hpp>

class GpuPool
{
public:
	GpuPool();

	bool exists() const;
	const u32 size() const;
	const u32 capacity() const;

private:
	u32 capacity;
	u32 size;
};