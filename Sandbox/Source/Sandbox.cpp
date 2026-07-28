#include <Entix/Game/EntryPoint.h>
#include <Entix/Game/Application.h>

#include <Entix/RHI/Shader.h>

namespace Entix
{
    class SandboxApp : public Application
    {
    public:
        SandboxApp()
            : Application(ApplicationConfig())
        {
            if(auto res = RHI::ShaderCompiler::Instance()->Compile("C:\\Users\\User\\Desktop\\Entix-Engine\\Shaders\\SimpleShader.slang"); res.IsError())
            {
                EX_LOG(ShaderCompilation, Fatal, "{}", res.UnwrapErr());
            }
        }
    };

    Application* CreateApplication()
    {
        return new SandboxApp();
    }
}