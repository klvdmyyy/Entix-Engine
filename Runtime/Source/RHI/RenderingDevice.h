#pragma once

#include "ERuntimeExport.h"

#include "RHI/RenderingContext.h"

#include <string>
#include <cstddef>

namespace RHI
{
	enum class GraphicsAPI
	{
		None = 0,
		OpenGL,
		Vulkan,
		D3D12,
	};

	inline std::string GraphicsAPIToString(GraphicsAPI api) noexcept
	{
#define ADD_API(X) case GraphicsAPI::X: return #X
		switch (api)
		{
			ADD_API(OpenGL);
			//ADD_API(Vulkan);
			//ADD_API(D3D12);

		case GraphicsAPI::None:
			return "<NO_GRAPHICS_API>";

		default:
			return "<UNSUPPORTED_OR_UNKNOWN_GRAPHICS_API>";
		}
#undef ADD_API
	}

	class ERUNTIME_EXPORT RenderingDevice
	{
	public:
		virtual ~RenderingDevice() = default;

		virtual RenderingContext* CreateContext(Core::Window& window) = 0;

		/* Static methods */

		static RenderingDevice* Create(GraphicsAPI api, size_t index = 0);

		static GraphicsAPI GetAPI() noexcept
		{
			return s_API;
		}

	protected:
		RenderingDevice() = default;

#ifdef NDEBUG
		static constexpr bool ENABLE_VALIDATION_LAYERS = 0;
#else
		static constexpr bool ENABLE_VALIDATION_LAYERS = 1;
#endif

	private:
		static GraphicsAPI s_API;
	};
}