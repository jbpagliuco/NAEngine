#pragma once

namespace na
{
	extern float Frametime;

	bool InitializeEngine();
	void ShutdownEngine();

	void DoFrame();
}