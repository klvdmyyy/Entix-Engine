// -*- mode: c++; -*-
#pragma once

#include "Core/TypeName.h"
#include "Core/IO/FileReader.h"

#include "Resources/ResourceHandle.h"
#include "Resources/ResourceLoader.h"

#include <queue>
#include <unordered_map>
#include <thread>
#include <mutex>

#include <concepts>

class ResourceManager {
public:
    static ResourceManager& Instance();

    enum class LoadMode {
        Sync,
        Async,
    };

    template<typename T>
    void RegisterLoader(Scope<ResourceLoader> loader)
    {
        EX_LOG(Trace, LogCategory::Resource, "Registering new resource loader: {}", TypeName<T>());
        m_loaders[typeid(T).hash_code()] = std::move(loader);
    }

    template<typename T, typename... Args>
    void RegisterLoader(Args&&... args)
    {
        this->RegisterLoader<T>(Scope<ResourceLoader>(new T(std::forward<Args>(args)...)));
    }

    template<std::derived_from<Resource> T, std::derived_from<ResourceLoader> Loader>
    [[nodiscard]]
    ResourceHandle<T> Load(const std::filesystem::path& path)
    {
        ResourceId id(path);
        return LoadInternal<T, Loader>(id, LoadMode::Sync);
    }

    template<std::derived_from<Resource> T, std::derived_from<ResourceLoader> Loader>
    [[nodiscard]]
    ResourceHandle<T> LoadAsync(const std::filesystem::path& path)
    {
        ResourceId id(path);
        return LoadInternal<T, Loader>(id, LoadMode::Async);
    }

    void Unload(const ResourceId& id);

    template<std::derived_from<ResourceLoader> T>
    void UnloadAll()
    {
        EX_ASSERT(m_loaders.contains(typeid(T).hash_code()), "Using unregistered loader for resource. Loader: '{}'", TypeName<T>());

        auto& loader = m_loaders[typeid(T).hash_code()];
        for(const ResourceId& id : loader.GetAllIds()) {
            Unload(id);
        }
    }

    [[nodiscard]]
    size_t GetTotalMemoryUsage() const;

    [[nodiscard]]
    size_t GetResourceCount() const;

private:
    struct LoadRequest {
    };

    void LoadStart();
    void LoadEnd();

    template<std::derived_from<Resource> T, std::derived_from<ResourceLoader> Loader>
    ResourceHandle<T> LoadInternal(const ResourceId& id, [[maybe_unused]] LoadMode mode)
    {
        this->LoadStart();

        EX_ASSERT(m_loaders.contains(typeid(Loader).hash_code()), "Using unregistered loader for resource. Loader: '{}'", TypeName<Loader>());

        if(!m_resources.contains(id)) {
            EX_LOG(Trace, LogCategory::Resource, "Loading '{}' using {}", (String)id, TypeName<Loader>());
            Scope<ResourceLoader>& loader = m_loaders.at(typeid(Loader).hash_code());

            auto fileReader = CreateScope<IO::FileReader>((std::filesystem::path)id);

            m_resources[id] = loader->LoadInternal(*fileReader, id);
        } else {
            EX_LOG(Trace, LogCategory::Resource, "'{}' is already loaded. Just returning it!", (String)id);
        }

        auto res = dynamic_cast<T*>(m_resources[id].get());
        EX_ASSERT(res, "Failed to cast resource provided by loader to '{}' type!", TypeName<T>());

        res->SetState(Resource::State::Ready);

        this->LoadEnd();
        return ResourceHandle<T>(res->GetId(), res);
    }

    std::unordered_map<ResourceId, Scope<Resource>, ResourceId::Hasher> m_resources;
    std::unordered_map<size_t, Scope<ResourceLoader>> m_loaders;

    std::queue<LoadRequest> m_loadQueue;
    std::thread m_loaderThread;
    std::mutex m_sync;
};