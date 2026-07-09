// -*- mode: c++; -*-
#pragma once

#include "Resources/Handle.h"

#include <queue>
#include <unordered_map>
#include <thread>
#include <mutex>

struct ResourceLoader {
    virtual ~ResourceLoader() = default;
    virtual Scope<Resource> Load(IO::Reader& reader, const ResourceId& id) = 0;
};

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
        m_loaders[typeid(T).hash_code()] = std::move(loader);
    }

    template<std::derived_from<Resource> T>
    ResourceHandle<T> Load(const std::filesystem::path& path)
    {
        ResourceId id(path);
        return LoadInternal<T>(id, LoadMode::Sync);
    }

    template<std::derived_from<Resource> T>
    ResourceHandle<T> LoadAsync(const std::filesystem::path& path)
    {
        ResourceId id(path);
        return LoadInternal<T>(id, LoadMode::Async);
    }

    void CollectGarbage();

    void Unload(const ResourceId& id);

    template<typename T>
    void UnloadAll()
    {
    }

    bool IsReady(const ResourceId& id) const;

    size_t GetTotalMemoryUsage() const;
    size_t GetResourceCount() const;

private:
    template<typename T>
    ResourceHandle<T> LoadInternal(const ResourceId& id, LoadMode mode)
    {
    }

    std::unordered_map<ResourceId, Scope<Resource>, ResourceId::Hasher> m_resources;
    std::unordered_map<size_t, Scope<ResourceLoader>> m_loaders;

    std::queue<LoadRequest> m_loadQueue;
    std::thread m_loaderThread;
    std::mutex m_sync;
};

void test()
{
    auto& rm = ResourceManager::Instance();
    
    auto squareTexture = rm.LoadAsync<Texture>("Assets/Textures/Default.texture");
    auto firstPBRMaterial = rm.LoadAsync<Material>("Assets/Materials/FirstPBR.material");
    auto simpleShader = rm.LoadAsync<Shader>("Assets/Shaders/Default.shader");

    simpleShader.WaitUntilReady();
}