#include <cstdlib>

#include <Windows.h>

#include "Base/Util/Serialize.h"
#include "Engine/Engine.h"
#include "Engine/World/World.h"

#if defined(_NA_TOOLS)
#include "Tools.h"
#endif

#if !defined(_NA_TOOLS)

static int GameMain()
{
	na::DeserializationParameterMap config = na::ParseFile("config.xml");

	// Initialize
	if (na::InitializeEngine() == false) {
		return EXIT_FAILURE;
	}
	
	na::LoadWorldFromFile(config["world"].AsString());

	// Run
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	while (true) {
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT) {
			break;
		}

		na::DoFrame();
	}

	// Shutdown
	na::ShutdownEngine();

	// Success!
	return 0;
}

#endif

int main(int argc, char *argv[])
{
#if defined(_NA_TOOLS)
	return ToolsMain();
#else
	return GameMain();
#endif
}