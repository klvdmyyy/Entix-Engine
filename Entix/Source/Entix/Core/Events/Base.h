#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Types.h"

#define EX_DEFINE_EVENT_TYPE(type) \
    public: \
    static const char* GetStaticType() { return #type; } \
    virtual const char* GetType() const noexcept override { return GetStaticType(); } \
    virtual Event* Clone() const override { return new type(*this); }

#define EX_DEFINE_EVENT_TYPE_CATEGORY(type, categoryFlags) \
    EX_DEFINE_EVENT_TYPE(type) \
    public: \
    virtual Int32 GetCategoryFlags() const noexcept override { return static_cast<Int32>(categoryFlags); }

namespace Entix
{
    enum class EventCategory
    {
        None = 0,
        Application = 1 << 0,
        Input = 1 << 1,
        Keyboard = 1 << 2,
        Mouse = 1 << 3,
        MouseButton = 1 << 4,
        Window = 1 << 5,
        Resources = 1 << 6,
    };

    inline Int32 operator|(EventCategory f, EventCategory s)
    {
        return static_cast<Int32>(f) | static_cast<Int32>(s);
    }

    inline Int32 operator|(Int32 f, EventCategory s)
    {
        return f | static_cast<Int32>(s);
    }

    inline Int32 operator&(Int32 f, EventCategory s)
    {
        return f & static_cast<Int32>(s);
    }

    class Event
    {
    public:
        virtual ~Event() = default;

        virtual const char* GetType() const noexcept = 0;
        virtual Event* Clone() const = 0;

        virtual Int32 GetCategoryFlags() const noexcept { return static_cast<Int32>(EventCategory::None); }

        EX_FORCE_INLINE
        inline bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    };
}