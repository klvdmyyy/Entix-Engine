#pragma once

#include "Core/Window.h"

namespace RHI
{
	class RenderingContext
	{
	public:
		virtual ~RenderingContext() = default;

	protected:
		RenderingContext() = default;
	};
}