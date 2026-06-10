#include "GameMain.h"

#include <Core/Application.h>
#include <Core/Log.h>

#include <RHI/RenderingDevice.h>

int GameMain()
{
	CollectArgs();

	Core::ApplicationSpecification appSpec =
	{
		.Name = "Entix Example Game",
		.Description = "My first game written with my own Game Engine",
		.WindowSpec = { .Width = 800, .Height = 600, .Title = "The Game!" },
	};

	Core::Application app(appSpec, argc, argv);

	RHI::RenderingDevice* device = RHI::RenderingDevice::Create(RHI::GraphicsAPI::Vulkan);

	app.Run();

	delete device;
	return 0;
}