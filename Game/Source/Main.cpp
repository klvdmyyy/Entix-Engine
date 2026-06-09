
#include <Core/Application.h>

#ifdef _WIN32
#include <Windows.h>
#define GameMain() WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int mShowCmd)
#else
#define GameMain() main()
#endif

int GameMain()
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