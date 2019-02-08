#include <cstdlib>

#include <Windows.h>

#include "Core/Engine/Engine.h"

int main(int argc, char *argv[])
{
	// Initialize
	if (na::InitializeEngine() == false) {
		return EXIT_FAILURE;
	}

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

		// TODO: Remove once the main window is in.
		if (GetAsyncKeyState(VK_ESCAPE)) {
			break;
		}

		na::DoFrame();
	}

	// Shutdown
	na::ShutdownEngine();

	system("pause");

	// Success!
	return 0;
}