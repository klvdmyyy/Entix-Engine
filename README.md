
# Entix Engine

[![Discord](https://img.shields.io/badge/Discord-5865F2?style=for-the-badge&logo=discord&logoColor=white)](https://discord.gg/9jCmeAGZtj)
[![Boosty](https://img.shields.io/badge/Boosty-FF6600?style=for-the-badge&logo=boosty&logoColor=white)](https://boosty.to/entixengine)

**Entix Engine is a powerfull 3D game engine written in modern C++ with monolithic architecture.**

## Table Of Content

- [Features](#features)
	- [Rust-like error handling](#rust-like-error-handling)
    - [Simple serialization](#simple-serialization)
	- [Async Resource Manager](#async-resource-manager)
	- [RHI (Rendering Hardware Interface)](#rhi-rendering-hardware-interface)

## Features

### Rust-like error handling

```c++
#include <Entix/Core/Result.h>

namespace Entix
{
	struct MyErrorType
	{
		String message;
	};

	// `Result<void>` is same with `Result<void, Error>`
	Result<void> UnstableFunction(Int32 value)
	{
		if(value == 0) return {};
		else return Error("Hello!!!");
	}

	Result<void, MyErrorType> SoUnstableFunction()
	{
		return MyErrorType { .message = "Privet!!!" };
	}

	Result<Int32> StableFunction()
	{
		return 10;
	}

	Result<void> Testing()
	{
		UnstableFunction(0).Unwrap();
		SoUnstableFunction().UnwrapErr();
		StableFunction().Unwrap();

		EX_TRY(UnstableFunction(0));

		// We can't do this because of different error types!
		// EX_TRY(StableFunction());

		// If result is success, write return value to `stableValue` variable
		EX_LET_TRY(stableValue, StableFunction());

		// `Testing()` returns error from `SoUnstableFunction()`
		EX_TRY(SoUnstableFunction());
	}
}
```

### Simple serialization

You need to write following code to make your data serializable:

```c++
#include <Entix/Serialization/JsonArchive.h>

namespace Entix
{
	struct MyDataStructure
	{
		Int32 firstValue;
		String secondValue;
		TransformComponent transform; // <- All built-in data structures are serializable
		// and e.t.c.
		
		void Serialize(JsonArchive& ar)
		{
			ar & AField("first", firstValue)
			& AField("second", secondValue)
			& AField("transform", transform);
		}
		
		static MyDataStructure Deserialize(JsonArchive& ar)
		{
			Int32 first;
			String second;
			TransformComponent transform;
			
			ar & AField("first", first)
			& AField("second", second);
			& AField("transform", transform);
			
			return MyDataStructure {
				.firstValue = first,
				.secondValue = second,
				.transform = transform
			};
		}
	};
}
```

### Async Resource Manager

```c++
#include <Entix/Resources/ResourceManager.h>

namespace Entix
{
	void YourFunction()
	{
		auto rm = ResourceManager::Instance();

		ResourceHandle<Texture> tex = rm.Load<Texture>(ResourceId("your_so_big_texture.png"));

		// Wait until your texture is ready!
		(void)*tex;
	}
}
```

### RHI (Rendering Hardware Interface)
