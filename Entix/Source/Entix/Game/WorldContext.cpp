#include "Entix/Game/WorldContext.h"

namespace Entix
{
    WorldContext::WorldContext(
        ThreadPool& threadPool,
        ResourceManager& resourceManager,
        const Ref<RHI::Device>& renderingDevice
    ) : m_threadPool(threadPool),
        m_resourceManager(resourceManager),
        m_renderingDevice(renderingDevice)
    {
    }
}
