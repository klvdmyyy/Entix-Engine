#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Result.h"
#include "Entix/Core/Events/Base.h"

#include "Entix/Resources/ResourceId.h"

namespace Entix
{
    class ResourceReloadedEvent : public Event
    {
        EX_DEFINE_EVENT_TYPE_CATEGORY(ResourceReloadedEvent, EventCategory::Resources);

    public:
        ResourceReloadedEvent(const ResourceId resourceId) : m_resourceId(resourceId) {}

        const ResourceId& GetResourceId() const noexcept { return m_resourceId; }

    private:
        ResourceId m_resourceId;
    };

    class Resource
    {
    public:
        explicit Resource(const ResourceId& id) : m_resourceId(id) {}
        virtual ~Resource() = default;

        const ResourceId& GetId() const { return m_resourceId; }
        bool IsLoaded() const { return m_loaded; }

        ENTIX_API Result<void> Load();
        ENTIX_API void Unload();

    protected:
        virtual Result<void> LoadInternal() = 0;
        virtual void UnloadInternal() = 0;

    private:
        bool m_loaded = false;
        ResourceId m_resourceId;
    };
}