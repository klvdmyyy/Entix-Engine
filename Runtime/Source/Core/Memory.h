#pragma once

#include <memory>

namespace Core
{
	/* Scoped pointer type (alias for std::unique_ptr) */

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename... Args>
	inline Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	/* Ref pointer type (alias for std::shared_ptr) */

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	inline Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}