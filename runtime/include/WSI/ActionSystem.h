// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/String.h"
#include "Core/Memory.h"

#include "WSI/Input.h"

#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <filesystem>

namespace ERUNTIME_NAMESPACE {
    // ---------------------------------------------------------------------
    // Привязки для действий. Используются в качестве абстракции над разными
    // типами ввода (клавиатура, геймпад и т.д)
    // ---------------------------------------------------------------------
    struct ActionBinding {
        InputDevice device;
        uint8_t scancode;
    };

    // ---------------------------------------------------------------------
    // Карта действий (action). Сопостовляет название действия (action name)
    // с определённой привязкой (action binding)
    // ---------------------------------------------------------------------
    class ActionMap {
    public:
        ActionMap() = default;

        void AddAction(const String& name, const ActionBinding& binding);

        [[nodiscard]]
        bool IsPressed(const String& name) const;

        [[nodiscard]]
        static ActionMap LoadFromFile(const std::filesystem::path filepath);
        void SaveToFile(const std::filesystem::path filepath);

    private:
        std::unordered_map<String, ActionBinding> m_bindingMap{};
    };

    // ---------------------------------------------------------------------
    // Контекст действий. Позволяет фильтровать обрабатываемые действия.
    //
    // Контекст решает в какой момент какие действия нужно обрабатывать,
    // а какие просто игнорировать.
    // ---------------------------------------------------------------------
    class ActionContext {
    public:
        // ---------------------------------------------------------------------
        // Инициализируется при помощи `std::initializeer_list<String>`.
        //
        // Нужно просто передать список обрабатываемых действий в текущем контексте
        // ---------------------------------------------------------------------
        ActionContext(const std::initializer_list<String>& actions)
            : m_actionSet(actions)
        {
        }

        // ---------------------------------------------------------------------
        // Добавить действие в контекст.
        //
        // Желательно указывать все нужные действия при инициализации, и не
        // использовать данный метод (Хотя ничего не мешает это делать,
        // и это не будет ошибкой).
        // ---------------------------------------------------------------------
        void AddAction(const String& name);

        [[nodiscard]]
        bool HasAction(const String& name) const;

    private:
        std::unordered_set<String> m_actionSet{};
    };

    // ---------------------------------------------------------------------
    // Система действий. Глобальный класс для обработки действий
    // ---------------------------------------------------------------------
    class ActionSystem {
    public:
        static ActionSystem& Instance();

        void PushContext(const ActionContext& context);
        void PopContext();

        void SetActionMap(const ActionMap& map);

        [[nodiscard]]
        bool IsPressed(const String& name) const;

    private:
        ActionMap m_actionMap{};
        std::stack<ActionContext> m_contextStack{};
    };
}