#include "GameMain.h"

#include <Core/Application.h>

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

	app.Run();

	return 0;
}