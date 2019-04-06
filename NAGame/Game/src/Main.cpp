#include <cstdlib>

#include <Windows.h>

#include "Core/Engine/Engine.h"

#include "Core/World/World.h"

int main(int argc, char *argv[])
{
	// Initialize
	if (na::InitializeEngine() == false) {
		return EXIT_FAILURE;
	}

	na::LoadWorldFromFile("data\\world.xml");

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