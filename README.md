
# Entix Engine

[![Discord](https://img.shields.io/badge/Discord-5865F2?style=for-the-badge&logo=discord&logoColor=white)](https://discord.gg/9jCmeAGZtj)
[![Boosty](https://img.shields.io/badge/Boosty-FF6600?style=for-the-badge&logo=boosty&logoColor=white)](https://boosty.to/entixengine)

**Entix Engine is a powerfull 3D game engine written in modern C++ with monolithic architecture.**

## Table Of Content

- [Features](#features)
    - [Simple serialization](#simple-serialization)
	- [RHI (Rendering Hardware Interface)](#rhi-rendering-hardware-interface)

## Features

### Simple serialization

You need to write following code to make your data serializable:

```c++
#include <Entix/Serialization/JsonArchive.h>

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
```

### RHI (Rendering Hardware Interface)
