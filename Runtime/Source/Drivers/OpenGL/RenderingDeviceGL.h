#pragma once

#include "RHI/RenderingDevice.h"

namespace Drivers
{
	class RenderingDeviceGL : public RHI::RenderingDevice
	{
	public:
		RenderingDeviceGL(size_t index);
		~RenderingDeviceGL();

	private:
		const size_t m_Index;
	};
}