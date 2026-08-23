#include <renderer/gpu/resource/material.hpp>
#include <renderer/layer/backend.hpp>

void Material::bind() const
{
    Backend::bind_material(*this);
}