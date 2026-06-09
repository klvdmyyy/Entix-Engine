#pragma once

namespace Core
{
	class Event
	{
	public:
		virtual ~Event() = default;

		virtual const char* GetType() const = 0;
		virtual Event* Clone() const = 0;
	};
}

/* Macro to help define event types */
#define DEFINE_EVENT_TYPE(type)                                         \
  static const char* GetStaticType() { return #type; }			\
  virtual const char* GetType() const override { return GetStaticType(); } \
  virtual ::Core::Event* Clone() const override { return new type(*this); }