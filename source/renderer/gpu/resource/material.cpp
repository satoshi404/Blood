#include <renderer/gpu/resource/material.hpp>
#include <renderer/backend/backend.hpp>

void Material::bind() const
{
    Backend::bind_material(*this);
}