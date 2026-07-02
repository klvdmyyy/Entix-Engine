#include "Core/Types.h"

#include <random>

static std::random_device s_randomDevice;
static std::mt19937_64 s_engine(s_randomDevice());
static std::uniform_int_distribution<uint64_t> s_uniformDistribution;

UUID::UUID()
    : m_uuid(s_uniformDistribution(s_engine))
{
}

UUID::UUID(Uint64 uuid)
    : m_uuid(uuid)
{
}