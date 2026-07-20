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
    ResourceManager();

    static ResourceManager& Instance();

    FORCE_INLINE
    inline void SetAssetsDirectory(const std::filesystem::path& path) noexcept
    {
        m_assetDir = path;
    }

    // Use same method for getting full asset filepath in ResourceManager!
    //
    // Different filepath produce different hashes in ResourceId. Just using '/' instead of '\' is
    // already can cause a lot of errors.
    //
    // TODO: We need to always replace all '\' with '/' symbol
    FORCE_INLINE
    inline std::filesystem::path GetAsset(const std::filesystem::path& path) noexcept
    {
        return m_assetDir / path;
    }

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
        ResourceId id(GetAsset(path));
        return LoadInternal<T, Loader>(id, LoadMode::Sync);
    }

    template<std::derived_from<Resource> T, std::derived_from<ResourceLoader> Loader>
    [[nodiscard]]
    ResourceHandle<T> LoadAsync(const std::filesystem::path& path)
    {
        ResourceId id(GetAsset(path));
        return LoadInternal<T, Loader>(id, LoadMode::Async);
    }

    void Reload(const ResourceId& id);

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
        std::lock_guard<std::mutex> guard(m_sync);

        // Start load tracing
        this->LoadStart();

        // Check if required loader exists.
        EX_ASSERT(m_loaders.contains(typeid(Loader).hash_code()), "Using unregistered loader for resource. Loader: '{}'", TypeName<Loader>());

        // Check if required resource already exists
        if(!m_resources.contains(id)) {
            EX_LOG(Trace, LogCategory::Resource, "Loading '{}' using {}", (String)id, TypeName<Loader>());
            if(mode == LoadMode::Async) {
                EX_LOG(Error, LogCategory::Resource, "Async resource loading is unimplemented for now. Resource loading are skipped. Handle object of resource can throw the assertion!");
                this->LoadEnd();
                return ResourceHandle<T>();
            } else {
                Scope<ResourceLoader>& loader = m_loaders.at(typeid(Loader).hash_code());

                auto fileReader = CreateScope<IO::FileReader>((std::filesystem::path)id);

                m_resources[id] = loader->LoadInternal(*fileReader, id);
            }
        } else {
            EX_LOG(Trace, LogCategory::Resource, "'{}' is already loaded. Just returning it!", (String)id);
        }

        // Check for resource type mismatches
        T* res = dynamic_cast<T*>(m_resources[id].get());
        EX_ASSERT(res, "Failed to cast resource provided by loader to '{}' type!", TypeName<T>());

        res->SetLoaderType(typeid(Loader));
        res->SetResourceType(typeid(T));

        // Set resource as Ready To Use
        res->SetState(Resource::State::Ready);

        // End load tracing
        this->LoadEnd();

        // Return the resource handle object
        return ResourceHandle<T>(res->GetId(), res);
    }

    std::unordered_map<ResourceId, Scope<Resource>, ResourceId::Hasher> m_resources;
    std::unordered_map<size_t, Scope<ResourceLoader>> m_loaders;

    std::queue<LoadRequest> m_loadQueue;
    std::thread m_loaderThread;
    std::mutex m_sync;

    // Asset dir
    std::filesystem::path m_assetDir;
};