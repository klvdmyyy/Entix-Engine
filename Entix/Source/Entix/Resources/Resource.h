#pragma once

#include "Entix/Core/Result.h"

#include "Entix/Resources/ResourceId.h"

namespace Entix
{
    class Resource
    {
    public:
        explicit Resource(const ResourceId& id) : m_resourceId(id) {}
        virtual ~Resource() = default;

        const ResourceId& GetId() const { return m_resourceId; }
        bool IsLoaded() const { return m_loaded; }

        Result<void> Load()
        {
            auto res = LoadInternal();
            m_loaded = res.IsSuccess();
            return res;
        }

        void Unload()
        {
            if(!m_loaded) return;
            
            UnloadInternal();
            m_loaded = false;
        }

    protected:
        virtual Result<void> LoadInternal() = 0;
        virtual void UnloadInternal() = 0;

    private:
        bool m_loaded = false;
        ResourceId m_resourceId;
    };
}