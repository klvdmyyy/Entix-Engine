#include "Resources/ResourceManager.h"

#include <tracy/TracyC.h>

// Global TracyZone context for Resource Manager.
static TracyCZoneCtx g_traceZoneCtx;

ResourceManager &ResourceManager::Instance() {
  static ResourceManager s_instance;
  return s_instance;
}

void ResourceManager::Unload(const ResourceId &id) {}

size_t ResourceManager::GetTotalMemoryUsage() const { return size_t(); }

size_t ResourceManager::GetResourceCount() const { return size_t(); }

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
