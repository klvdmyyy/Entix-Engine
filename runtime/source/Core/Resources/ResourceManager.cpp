#include "Core/Resources/ResourceManager.h"

#include "Core/StringCommandRunner.h"

#include "Core/Assert.h"
#include "Core/Debug/Log.h"

namespace ERUNTIME_NAMESPACE {
    ResourceManager::ResourceManager()
    {
        StringCommandRunner::Instance()
            .AddCommand({ .name = "list_shaders", .description = "List all shaders" },
                        [&](const CommandArgs& args, IO::Writer& writer) {
                            writer.Write("SHADERS LIST:");
                            for(const auto [filepath, _] : m_shadersMap) {
                                writer.WriteFmt("\t{}", filepath);
                            }
                        });

        StringCommandRunner::Instance()
            .AddCommand({ .name = "load_shader", .description = "Loading shaders" },
                        [this](const CommandArgs& args, IO::Writer& writer) {
                            if(args.Count() != 1) {
                                writer.Write("Usage: load_shader <SHADER_FILEPATH>");
                                return;
                            }
                            this->LoadShader(String(args.Get(0)));
                        });

        // StringCommandRunner::Instance()
        //     .AddCommand({ .name = "unload_shader", .description = "Unloading shaders" },
        //                 [this](const CommandArgs& args, IO::Writer& writer) {
        //                     if(args.Count() != 1) {
        //                         writer.Write("Usage: unload_shader <SHADER_FILEPATH>");
        //                         return;
        //                     }
        //                     this->UnloadShader(String(args.Get(0)));
        //                 });
        
        StringCommandRunner::Instance()
            .AddCommand({ .name = "reload_shader", .description = "Reloading shaders" },
                        [this](const CommandArgs& args, IO::Writer& writer) {
                            if(args.Count() != 1) {
                                writer.Write("Usage: reload_shader <SHADER_FILEPATH>");
                                return;
                            }
                            this->ReloadShader(String(args.Get(0)));
                        });
    }

    ResourceManager& ResourceManager::Instance()
    {
        static ResourceManager s_instance;
        return s_instance;
    }

    const Ref<Renderer::Shader>& ResourceManager::LoadShader(const String& filepath)
    {
        Debug::Info(LogCategory::Resources, "Loading shader '{}'", filepath);
        auto searchIt = m_shadersMap.find(filepath);

        if(searchIt != m_shadersMap.end()) {
            Debug::Warn(LogCategory::Resources, "Failed to load shader '{}'. This shader was already loaded!", filepath);
            return searchIt->second;
        }

        auto loaded = Ref<Renderer::Shader>(m_rendererContext->CreateShader(filepath));

        m_shadersMap.insert({filepath, std::move(loaded)});

        return m_shadersMap.at(filepath);
    }

    const Ref<Renderer::Shader>& ResourceManager::GetShader(const String& filepath) const
    {
        return m_shadersMap.at(filepath);
    }

    void ResourceManager::ReloadShader(const String& filepath)
    {
        Debug::Info(LogCategory::Resources, "Reloading shader '{}'", filepath);

        auto searchIt = m_shadersMap.find(filepath);

        if(searchIt == m_shadersMap.end()) {
            Debug::Error(LogCategory::Resources, "Failed to reload shader '{}'. Shader doesn't exists!", filepath);
            return;
        }

        searchIt->second.reset(m_rendererContext->CreateShader(filepath));
    }
}

