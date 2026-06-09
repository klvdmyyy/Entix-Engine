#pragma once

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

	class RenderingDevice
	{
	public:
		virtual ~RenderingDevice() = default;

		static GraphicsAPI GetAPI() noexcept
		{
			return s_API;
		}

		static RenderingDevice* Create(GraphicsAPI api, size_t index = 0);

	private:
		RenderingDevice() = default;

		static GraphicsAPI s_API;
	};
}