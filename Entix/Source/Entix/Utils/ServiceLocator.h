#pragma once

#include "Entix/Core/Panic.h"
#include "Entix/Core/Memory.h"
#include "Entix/Core/TypeName.h"

#include <any>
#include <type_traits>
#include <unordered_map>
#include <typeindex>

namespace Entix
{
    class ServiceLocator
    {
    public:
        template<typename T, typename... Args>
            requires (!std::is_trivial_v<T> && std::is_constructible_v<T, Args>)
        static void Register(Args&&... args)
        {
            GetServices[std::type_index(typeid(T))] = CreateScope<std::any>(std::any(T(std::forward<Args>(args)...)));
        }

        template<typename T>
            requires (!std::is_trivial_v<T>)
        static T& Get()
        {
            auto it = GetServices().find(std::type_index(typeid(T)));

            if(it == GetServices.end())
                Panic(std::format("Trying to get unregister service from ServiceLocator: '{}'", TypeName<T>()));

            auto* ptr = std::any_cast<T*>(it->second.get());

            return *ptr;
        }

        template<typename T>
            requires (!std::is_trivial_v<T>)
        static bool Has()
        {
            return GetServices().find(std::type_index(typeid(T))) != GetServices().end();
        }

    private:
        static std::unordered_map<std::type_index, Scope<std::any>>& GetServices()
        {
            static std::unordered_map<std::type_index, Scope<std::any>> s_services;
            return s_services;
        }
    };
}
