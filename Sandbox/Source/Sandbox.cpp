#include <Entix/Game/EntryPoint.h>
#include <Entix/Game/Application.h>

#include <Entix/RHI/Shader.h>

#include <Entix/Core/Tasks/ThreadPool.h>

#include <Entix/Resources/ResourceManager.h>

namespace Entix
{
    class TestResource : public Resource
    {
    public:
        TestResource(const ResourceId& id) : Resource(id) {}

        int value;

    protected:
        Result<void> LoadInternal() final
        {
            static int nextValue = 10;
            value = nextValue;
            nextValue = nextValue * 2;

            std::this_thread::sleep_for(std::chrono::seconds(3));
            
            return {};
        }

        void UnloadInternal() final {}
    };

    class SandboxApp : public Application
    {
    public:
        SandboxApp()
            : Application(ApplicationConfig())
        {
            auto rm = ResourceManager();

            ResourceId testId("asd");
            auto resource = rm.Load<TestResource>(testId);

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

            ResourceId testId2("asd2");
            auto resource2 = rm.Load<TestResource>(testId2);

            EX_LOG(LogTemp, Warning, "RESOURCE MANAGER TEST: {}", resource->value);
            EX_LOG(LogTemp, Warning, "RESOURCE MANAGER TEST: {}", resource2->value);
        }
    };

    Application* CreateApplication()
    {
        return new SandboxApp();
    }
}
