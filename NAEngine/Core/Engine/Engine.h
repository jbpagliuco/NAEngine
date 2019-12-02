#pragma once

#include <stdint.h>

namespace na
{
	bool InitializeEngine();
	void ShutdownEngine();

	void DoFrame();

	float GetDeltaFrameTime();
	double GetElapsedTime();
}