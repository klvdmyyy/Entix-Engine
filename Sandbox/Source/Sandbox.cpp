#include <Entix/Game/EntryPoint.h>
#include <Entix/Game/Application.h>

#include <Entix/RHI/Shader.h>

#include <Entix/Core/Tasks/ThreadPool.h>

namespace Entix
{
    class SandboxApp : public Application
    {
    public:
        SandboxApp()
            : Application(ApplicationConfig())
        {

            std::vector<std::future<int>> results;

            for(int i = 0; i < 8; i++)
            {
                auto future = ThreadPool::Instance().Enqueue([i](){
                    EX_LOG(LogTemp, Info, "Calculating {}", i);
                    return i + i;
                });
                results.emplace_back(std::move(future));
            }

            for(auto& result : results)
                EX_LOG(LogTemp, Warning, "THREAD POOL TESTING: {}", result.get());
        }
    };

    Application* CreateApplication()
    {
        return new SandboxApp();
    }
}