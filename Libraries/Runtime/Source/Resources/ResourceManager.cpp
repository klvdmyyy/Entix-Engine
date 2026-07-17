#include "Resources/ResourceManager.h"

#include <tracy/TracyC.h>

#define RM_FRAME_NAME "Loading resources"

static TracyCZoneCtx g_traceZoneCtx;

ResourceManager &ResourceManager::Instance() {
  static ResourceManager s_instance;
  return s_instance;
}

void ResourceManager::Unload(const ResourceId &id) {}

size_t ResourceManager::GetTotalMemoryUsage() const { return size_t(); }

size_t ResourceManager::GetResourceCount() const { return size_t(); }

void ResourceManager::LoadStart()
{
  TracyCZone(ctx, true);
  g_traceZoneCtx = ctx;
}

void ResourceManager::LoadEnd()
{
  TracyCZoneEnd(g_traceZoneCtx);
}
