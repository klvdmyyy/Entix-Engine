#pragma once

#include "ERuntimeExport.h"

#include "Core/Window.h"

#include "RHI/RenderingContext.h"

namespace Drivers
{
	class ERUNTIME_EXPORT RenderingContextGL : public RHI::RenderingContext
	{
	public:
		RenderingContextGL(Core::Window& window);
		~RenderingContextGL();

	private:
		Core::Window& m_Window;
	};
}