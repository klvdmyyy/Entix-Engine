#include "WSI/ActionSystem.h"

#include "Core/Debug/Log.h"

#include <vector>
#include <fstream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ERUNTIME_NAMESPACE {
    void ActionMap::AddAction(const String& name, const ActionBinding& binding)
    {
        if(m_bindingMap.contains(name)) {
            Debug::Warn(LogCategory::WSI, "Trying to add an already existing action to ActionMap! Action name: {}", name);
            return;
        }

        m_bindingMap.insert({name, binding});
    }

    bool ActionMap::IsPressed(const String& name) const
    {
        if(!m_bindingMap.contains(name)) {
            Debug::Warn(LogCategory::WSI, "Trying to check unexisting action in ActionMap! Action name: {}", name);
            return false;
        }
        
        return Input::IsKeyPressed(m_bindingMap.find(name)->second.scancode);
    }

    ActionMap ActionMap::LoadFromFile(const std::filesystem::path filepath)
    {
        std::ifstream f(filepath);
        json data = json::parse(f);

        ActionMap actionMap;

        for(auto action : data["actions"].get<std::vector<json>>()) {
            actionMap.AddAction(action["name"].get<String>(),
                                ActionBinding {
                                    .device = InputDevice::Keyboard,
                                    .scancode = action["bindings"].get<std::vector<json>>()[0]["scancode"].get<Uint8>(),
                                });
            
        }
        
        return actionMap;
    }

    void ActionMap::SaveToFile(const std::filesystem::path filepath)
    {
        //...
    }

    void ActionContext::AddAction(const String& name)
    {
        if(m_actionSet.contains(name))
            Debug::Warn(LogCategory::WSI, "Trying to add an already existing action to ActionContext! Action name: {}", name);
        
        m_actionSet.insert(name);
    }

    bool ActionContext::HasAction(const String& name) const
    {
        return m_actionSet.contains(name);
    }

    ActionSystem& ActionSystem::Instance()
    {
        static ActionSystem s_instance;
        return s_instance;
    }

    void ActionSystem::PushContext(const ActionContext& context)
    {
        m_contextStack.push(context);
    }

    void ActionSystem::PopContext()
    {
        if(m_contextStack.empty()) {
            Debug::Warn(LogCategory::WSI, "Trying to call PopContext in ActionSystem when m_contextStack is empty!");
            return;
        }
        m_contextStack.pop();
    }

    void ActionSystem::SetActionMap(const ActionMap& map)
    {
        m_actionMap = map;
    }

    bool ActionSystem::IsPressed(const String& name) const
    {
        if (m_contextStack.empty() || !m_contextStack.top().HasAction(name))
            return false;

        return m_actionMap.IsPressed(name);
    }
}