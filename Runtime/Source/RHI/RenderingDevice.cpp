#include "RHI/RenderingDevice.h"

#include "Drivers/OpenGL/RenderingDeviceGL.h"

namespace RHI
{
	GraphicsAPI RenderingDevice::s_API = GraphicsAPI::None;

	RenderingDevice* RenderingDevice::Create(GraphicsAPI api, size_t index)
	{
		/* We can't create two or more devices! */
		if (s_API != GraphicsAPI::None)
			return nullptr;

		/* Set API static variable */
		s_API = api;

		/* Setting up the rendering device */
		switch (api)
		{
		case GraphicsAPI::None:
			return nullptr;

		case GraphicsAPI::OpenGL:
			return new Drivers::RenderingDeviceGL(index);

		case GraphicsAPI::Vulkan:
			return nullptr;

		case GraphicsAPI::D3D12:
			return nullptr;

		default:
			return nullptr;
		}
	}
}