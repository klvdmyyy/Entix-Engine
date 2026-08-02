#include "Entix/RHI/Device.h"

namespace Entix
{
    template<>
    RHI::GraphicsApi DefaultOf<RHI::GraphicsApi>()
    {
        return RHI::GraphicsApi::Vulkan;
    }

    template<>
    String ToString<RHI::GraphicsApi>(const RHI::GraphicsApi& api)
    {
        switch(api)
        {
            case Entix::RHI::GraphicsApi::Vulkan:
                return "Vulkan";
        }

        Panic();
    }

    template<>
    RHI::GraphicsApi FromString<RHI::GraphicsApi>(const String& str_)
    {
        String str = str_;
        std::transform(str.begin(), str.end(), str.begin(), [](char c){
            return std::tolower(c);
        });

        if(str == "vulkan") return RHI::GraphicsApi::Vulkan;

        EX_LOG(LogTemp, Warning, "Failed to convert `String -> RHI::GraphicsAPI`. Returns default value!");
        return DefaultOf<RHI::GraphicsApi>();
    }
}