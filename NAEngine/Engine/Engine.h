#pragma once

#include <stdint.h>

#define ENGINE_LOG_FILTER "Engine"

namespace na
{
	bool InitializeEngine();
	void ShutdownEngine();

	void DoFrame();

	float GetDeltaFrameTime();
	double GetElapsedTime();
}