#include "Entix/Game/WorldContext.h"

namespace Entix
{
    WorldContext::WorldContext(
        ThreadPool& threadPool,
        ResourceManager& resourceManager
    ) : m_threadPool(threadPool),
        m_resourceManager(resourceManager),
        m_renderingDevice(nullptr)
    {
    }
}
