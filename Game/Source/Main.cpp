
#include <Core/Application.h>

int main()
{
	Core::ApplicationSpecification appSpec =
	{
		.Name = "Entix Example Game",
		.WindowSpec = { .Width = 800, .Height = 600, .Title = "The Game!" },
	};

	Core::Application app(appSpec);

	app.Run();

	return 0;
}