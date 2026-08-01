
# Entix Engine

**Entix Engine is a powerfull 3d game engine written in modern C++ with monolithic architecture.**

## Table Of Content

- [Features](#features)
    - [Simple serialization](#simple-serialization)

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