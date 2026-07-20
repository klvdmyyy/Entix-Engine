#include "Resources/ResourceManager.h"

#include "Core/StringCommandRunner.h"
#include "Core/IO/Decorators.h"

#include <tracy/Tracy.hpp>
#include <tracy/TracyC.h>

// Global TracyZone context for Resource Manager.
static TracyCZoneCtx g_traceZoneCtx;

ResourceManager::ResourceManager()
{
  StringCommandRunner::Instance().AddCommand({ .name = "res_reload", .description = "Resource hot-reload" },
  [&](const CommandArgs& args, IO::Writer& rawWriter) {
    auto writer = IO::TextWriter::CreateNonOwned(rawWriter);
    if(args.Count() != 1) {
      writer.Write("Usage: res_reload <RESOURCE_FILEPATH>");
      return;
    }

    auto filepath = args.Get(0);
    Reload(ResourceId(GetAsset(filepath)));
  });

  StringCommandRunner::Instance().AddCommand({ .name = "res_list", .description = "List resources" },
  [&](const CommandArgs& args, IO::Writer& rawWriter) {
    auto writer = IO::TextWriter::CreateNonOwned(rawWriter);
    for(const auto& pair : m_resources) {
      writer.WriteFmt("{} - {}", (String)pair.first, pair.first.Hash());
    }
  });
}

ResourceManager &ResourceManager::Instance() {
  static ResourceManager s_instance;
  return s_instance;
}

void ResourceManager::Unload(const ResourceId &id) {}

size_t ResourceManager::GetTotalMemoryUsage() const { return size_t(); }

size_t ResourceManager::GetResourceCount() const { return size_t(); }

void ResourceManager::Reload(const ResourceId& id)
{
  ZoneScoped;

  EX_LOG(Trace, LogCategory::Resource, "Hot-reloading resource: '{}'", (String)id);

  Resource* oldResource = nullptr;
  {
    auto it = m_resources.find(id);
    if(it == m_resources.end()) {
      EX_LOG(Error, LogCategory::Resource, "Failed to hot-reload resource '{}'. Resource isn't loaded!", (String)id);
      return;
    }
    oldResource = it->second.get();
    if(oldResource->GetState() != Resource::State::Ready) {
      EX_LOG(Error, LogCategory::Resource, "Failed to hot-reload resource '{}'. Resource isn't ready-to-use!", (String)id);
      return;
    }
  }

  std::type_index loaderType = oldResource->GetLoaderType();

  if(!m_loaders.contains(loaderType.hash_code())) {
    EX_LOG(Error, LogCategory::Resource, "Failed to hot-realod resource '{}'. Target resource loader '{}' wasn't found!", (String)id, loaderType.name());
    return;
  }

  Scope<ResourceLoader>& loader = m_loaders[loaderType.hash_code()];

  auto fileReader = CreateScope<IO::FileReader>((std::filesystem::path)id);

  auto newResource = loader->LoadInternal(*fileReader, id);
  if(!newResource) {
    EX_LOG(Error, LogCategory::Resource, "Failed to hot-reload resource '{}'. Can't create new resource!", (String)id);
    return;
  }

  newResource->SetState(Resource::State::Ready);

  oldResource->HotReload(std::move(newResource));
}

// We don't need to include that in header file. Just use it
// as additional methods

void ResourceManager::LoadStart()
{
  TracyCZoneN(ctx, "Resource Loading", true);
  g_traceZoneCtx = ctx;
}

void ResourceManager::LoadEnd()
{
  TracyCZoneEnd(g_traceZoneCtx);
}
