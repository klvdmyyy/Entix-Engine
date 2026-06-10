#include "RHI/RenderingDevice.h"

#include "Core/Log.h"

#include "Drivers/OpenGL/RenderingDeviceGL.h"

namespace RHI
{
	GraphicsAPI RenderingDevice::s_API = GraphicsAPI::None;

	RenderingDevice* RenderingDevice::Create(GraphicsAPI api, size_t index)
	{
		/* We can't create two or more devices! */
		E_ASSERT(s_API == GraphicsAPI::None, "Trying to create second RenderingDevice object. We can't create two or more devices!");

		/* Set API static variable */
		s_API = api;

		E_TRACE("Choosed graphics api: {}", GraphicsAPIToString(api));
		E_TRACE("Creating RenderingDevice object.");

		/* Setting up the rendering device */
		switch (api)
		{
		case GraphicsAPI::None:
			E_ASSERT(false, "GraphicsAPI::None isn't supported!");
			return nullptr;

		case GraphicsAPI::OpenGL:
			return new Drivers::RenderingDeviceGL(index);

		case GraphicsAPI::Vulkan:
			E_ASSERT(false, "GraphicsAPI::Vulkan isn't supported now!");
			return nullptr;

		case GraphicsAPI::D3D12:
			E_ASSERT(false, "GraphicsAPI::D3D12 isn't supported now!");
			return nullptr;

		default:
			E_ASSERT(false, "Unknown GraphicsAPI value!");
			return nullptr;
		}
	}
}