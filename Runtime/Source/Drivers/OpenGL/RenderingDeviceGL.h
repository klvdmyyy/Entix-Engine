#pragma once

#include "RHI/RenderingDevice.h"

#include "Drivers/OpenGL/RenderingContextGL.h"

namespace Drivers
{
	class RenderingDeviceGL : public RHI::RenderingDevice
	{
	public:
		RenderingDeviceGL(size_t index);
		~RenderingDeviceGL();

		RHI::RenderingContext* CreateContext(Core::Window& window) final;

	private:
		const size_t m_Index;
	};
}