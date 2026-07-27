#include "Entix/Resources/ResourceManager.h"

#include "Entix/Core/IO/TextStream.h"
// #include "Entix/Utils/StringCommandRunner.h"

namespace Entix
{
    // Global TracyZone context for Resource Manager.
    // static TracyCZoneCtx g_traceZoneCtx;

    ResourceManager::ResourceManager() {}

    ResourceManager& ResourceManager::Instance()
    {
        static ResourceManager s_instance;
        return s_instance;
    }

    void ResourceManager::Unload(const ResourceId& id) {}

    Bytes ResourceManager::GetTotalMemoryUsage() const
    {
        Bytes usage = 0;

        for (const auto& pair : m_resources)
        {
            usage += pair.second->GetMemorySize();
        }

        return usage;
    }

    size_t ResourceManager::GetResourceCount() const { return size_t(); }

    void ResourceManager::Reload(const ResourceId& id)
    {
        EX_LOG(
            Resources, Trace, "Hot-reloading resource: '{}'",
            std::filesystem::relative((std::filesystem::path)(id), m_assetDir)
                .string());

        Resource* oldResource = nullptr;
        {
            auto it = m_resources.find(id);
            if (it == m_resources.end())
            {
                EX_LOG(Resources, Error,
                       "Failed to hot-reload resource '{}'. Resource "
                       "isn't loaded!",
                       std::filesystem::relative((std::filesystem::path)(id),
                                                 m_assetDir)
                           .string());
                return;
            }
            oldResource = it->second.get();
            if (oldResource->GetState() != Resource::State::Ready)
            {
                EX_LOG(Resources, Error,
                       "Failed to hot-reload resource '{}'. Resource isn't "
                       "ready-to-use!",
                       std::filesystem::relative((std::filesystem::path)(id),
                                                 m_assetDir)
                           .string());
                return;
            }
        }

        std::type_index loaderType = oldResource->GetLoaderType();

        if (!m_loaders.contains(loaderType.hash_code()))
        {
            EX_LOG(Resources, Error,
                   "Failed to hot-realod resource '{}'. Target resource "
                   "loader '{}' "
                   "wasn't found!",
                   std::filesystem::relative((std::filesystem::path)(id),
                                             m_assetDir)
                       .string(),
                   loaderType.name());
            return;
        }

        Scope<ResourceLoader>& loader = m_loaders[loaderType.hash_code()];

        auto fileReader =
            CreateScope<IO::FileReader>((std::filesystem::path)id);

        auto newResource = loader->LoadInternal(*fileReader, id);
        if (!newResource)
        {
            EX_LOG(Resources, Error,
                   "Failed to hot-reload resource '{}'. Can't create new "
                   "resource!",
                   std::filesystem::relative((std::filesystem::path)(id),
                                             m_assetDir)
                       .string());
            return;
        }

        newResource->SetState(Resource::State::Ready);

        oldResource->HotReload(std::move(newResource));
    }

    // We don't need to include that in header file. Just use it
    // as additional methods

    void ResourceManager::LoadStart() {}

    void ResourceManager::LoadEnd() {}
} // namespace Entix