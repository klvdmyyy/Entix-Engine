#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Memory.h"
#include "Entix/Core/String.h"

#include <type_traits>

namespace Entix
{
    class Console
    {
    public:
        using CVarCallback = std::function<void()>;

        static Console* Get()
        {
            static Console s_instance;
            return &s_instance;
        }

        ENTIX_API void ExecuteCommand(const String& cmdLine);
        ENTIX_API void ExecuteCommand(const std::vector<String>& args);

        template<IndirectString T>
        void SetCVar(const String& name, const T& value)
        {
            return SetCVarInternal(name, ToString<T>(value));
        }

        EX_FORCE_INLINE
        inline void SetCVarCallback(const String& name, CVarCallback callback)
        {
            if(m_cVarCallbackMap.contains(name)) return;

            std::unique_lock lock(m_sync);
            m_cVarCallbackMap.insert({name, callback});
        }

        template<IndirectString T>
        T GetCVar(const String& name)
        {
            return FromString<T>(GetCVarInternal(name));
        }

    private:
        Console() = default;

        ENTIX_API void SetCVarInternal(const String& name, const String& value);
        ENTIX_API String GetCVarInternal(const String& name);

        static std::vector<String> SplitString(const String& line);

        std::unordered_map<String, String> m_cVarMap;
        std::unordered_map<String, CVarCallback> m_cVarCallbackMap;

        std::mutex m_sync;
    };

    // -----------------------------------------------------------------------------------
    // TODO Пока что `CachedCVar` поддерживает работу только с одним экземпляром
    // одной и той же переменной.
    //
    // При использовании одинаковых `CachedCVar` в двух разных местах, работать ничего
    // не будет.
    //
    // По идее это не необходимость, но может быть стоит реализовать ?!
    //
    // (This is russian language)
    // -----------------------------------------------------------------------------------
    template<typename T>
        requires (IndirectString<T>)
    class CachedCVar
    {
    public:
        CachedCVar(const String& name, const T& value)
            : k_name(name),
              m_value(value)
        {
            Initialize();
        }

        CachedCVar(const String& name)
            requires (HasDefaults<T>)
            : k_name(name),
              m_value(DefaultOf<T>())
        {
            Initialize();
        }

        CachedCVar& operator=(const T& value)
            requires (std::is_trivially_copyable_v<T> || std::is_copy_assignable_v<T>)
        {
            m_value = value;
            Console::Get()->SetCVar<T>(k_name, m_value);
            return *this;
        }

        T Get() const noexcept
            requires (std::is_trivially_copyable_v<T> || std::is_copy_assignable_v<T>)
        {
            if(m_dirty)
            {
                *m_dirty = false;
                m_value = Console::Get()->GetCVar<T>(k_name);
            }
            return m_value;
        }

        operator T() const noexcept
            requires (std::is_trivially_copyable_v<T> || std::is_copy_assignable_v<T>)
        {
            return Get();
        }

    private:
        void Initialize()
        {
            auto console = Console::Get();

            console->SetCVar<T>(k_name, m_value);
            console->SetCVarCallback(k_name, [weakDirty = WeakRef<bool>(m_dirty)]() {
                if(auto dirty = weakDirty.lock())
                {
                    *dirty = true;
                }
            });
        }

        mutable Ref<bool> m_dirty = CreateRef<bool>(false);

        const String k_name;
        mutable T m_value;
    };
}
