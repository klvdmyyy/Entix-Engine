#include "Input/ActionSystem.h"

#include "Core/Debug/Log.h"
#include "Core/StringCommandRunner.h"

#include "Core/IO/Decorators.h"

#include <vector>
#include <fstream>
#include <exception>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <nlohmann/json.hpp>

#pragma GCC diagnostic pop

#include <tracy/Tracy.hpp>

using json = nlohmann::json;

static constexpr const char* UPDATE_FRAME = "ActionSystem Update";

void ActionMap::Update()
{
    FrameMarkStart(UPDATE_FRAME);

    for(auto& [name, action] : m_actionDataMap) {
        bool rawPressed = Input::IsKeyPressed(action.binding.scancode);

        switch(action.state) {
        case ActionState::Idle:
            if(rawPressed) action.state = ActionState::Pressed;
            break;

        case ActionState::Pressed:
            action.state = rawPressed ? ActionState::Held : ActionState::Released;
            break;

        case ActionState::Held:
            if(!rawPressed) action.state = ActionState::Released;
            break;

        case ActionState::Released:
            action.state = rawPressed ? ActionState::Pressed : ActionState::Idle;
            break;
        }
    }

    FrameMarkEnd(UPDATE_FRAME);
}
    
void ActionMap::AddAction(const String& name, const ActionBinding& binding)
{
    ZoneScoped;

    if(m_actionDataMap.contains(name)) {
        EX_LOG(Warning, LogCategory::WSI, "Trying to add an already existing action to ActionMap! Action name: {}", name);
        return;
    }

    m_actionDataMap.insert({name, ActionData { .binding = binding }});
}

void ActionMap::Bind(const String& name, const ActionBinding& binding)
{
    ZoneScoped;

    auto it = m_actionDataMap.find(name);
        
    if(it == m_actionDataMap.end()) {
        AddAction(name, binding);
    } else {
        it->second.binding = binding;
    }
}

bool ActionMap::IsPressed(const String& name) const
{
    ZoneScoped;

    if(!m_actionDataMap.contains(name)) {
        EX_LOG(Warning, LogCategory::WSI, "Trying to check state of unexisting action in ActionMap! Action name: {}", name);
        return false;
    }

    auto it = m_actionDataMap.find(name);
    return it->second.state == ActionState::Pressed;
}

bool ActionMap::IsReleased(const String& name) const
{
    ZoneScoped;

    if(!m_actionDataMap.contains(name)) {
        EX_LOG(Warning, LogCategory::WSI, "Trying to check state of unexisting action in ActionMap! Action name: {}", name);
        return false;
    }

    auto it = m_actionDataMap.find(name);
    return it->second.state == ActionState::Released;
}

bool ActionMap::IsHeld(const String& name) const
{
    ZoneScoped;

    if(!m_actionDataMap.contains(name)) {
        EX_LOG(Warning, LogCategory::WSI, "Trying to check state of unexisting action in ActionMap! Action name: {}", name);
        return false;
    }

    auto it = m_actionDataMap.find(name);
    return it->second.state == ActionState::Pressed
        || it->second.state == ActionState::Held;
}

ActionMap ActionMap::LoadFromFile(const std::filesystem::path filepath)
{
    ZoneScoped;

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
    ZoneScoped;

    //...
}

void ActionContext::AddAction(const String& name)
{
    ZoneScoped;

    if(m_actionSet.contains(name))
        EX_LOG(Warning, LogCategory::WSI, "Trying to add an already existing action to ActionContext! Action name: {}", name);
        
    m_actionSet.insert(name);
}

bool ActionContext::HasAction(const String& name) const
{
    ZoneScoped;

    return m_actionSet.contains(name);
}

ActionSystem& ActionSystem::Instance()
{
    static ActionSystem s_instance;
    return s_instance;
}

void ActionSystem::PushContext(const ActionContext& context)
{
    ZoneScoped;

    m_contextStack.push(context);
}

void ActionSystem::PopContext()
{
    ZoneScoped;

    if(m_contextStack.empty()) {
        EX_LOG(Warning, LogCategory::WSI, "Trying to call PopContext in ActionSystem when m_contextStack is empty!");
        return;
    }
    m_contextStack.pop();
}

void ActionSystem::SetActionMap(const ActionMap& map)
{
    ZoneScoped;

    m_actionMap = map;
}

bool ActionSystem::IsPressed(const String& name) const
{
    ZoneScoped;

    if (m_contextStack.empty() || !m_contextStack.top().HasAction(name))
        return false;

    return m_actionMap.IsPressed(name);
}

bool ActionSystem::IsReleased(const String& name) const
{
    ZoneScoped;

    if (m_contextStack.empty() || !m_contextStack.top().HasAction(name))
        return false;

    return m_actionMap.IsReleased(name);
}

bool ActionSystem::IsHeld(const String& name) const
{
    ZoneScoped;
    
    if (m_contextStack.empty() || !m_contextStack.top().HasAction(name))
        return false;

    return m_actionMap.IsHeld(name);
}

ActionSystem::ActionSystem()
{
    StringCommandRunner::Instance()
        .AddCommand({ .name = "bind", .description = "Bind action to keyboard scancode." },
                    [&](const CommandArgs& args, IO::Writer& writer_) {
                        auto writer = IO::TextWriter::CreateNonOwned(writer_);
                        if(args.Count() != 2) {
                            writer.Write("Usage: bind <ACTION_NAME> <SCANCODE>");
                            return;
                        }
                        try {
                            m_actionMap.Bind(String(args.Get(0)), ActionBinding {
                                    .device = InputDevice::Keyboard,
                                    .scancode = static_cast<Uint8>(std::stoi(String(args.Get(1)))),
                                });
                        } catch(const std::exception& e) {
                            writer.WriteFmt("Error: {}", e.what());
                        }
                    });
}