#pragma once

#include "Resources/ResourceLoader.h"

#include "Renderer/Context.h"

class ObjMeshLoader : public ResourceLoader {
public:
    ObjMeshLoader(Renderer::Context* rendererContext) : m_rendererContext(rendererContext) {}

    Scope<Resource> Load(const ResourceId& id, IO::Reader& reader) final;

private:
    Renderer::Context* m_rendererContext;
};