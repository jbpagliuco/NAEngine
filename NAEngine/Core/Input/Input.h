#pragma once

#include "Base/Util/Util.h"

namespace na
{
	bool InputSystemInit();
	void InputSystemDoFrameLate();
	void InputSystemShutdown();

	bool IsKeyDown(int keyCode, bool ctrl = false, bool shift = false, bool alt = false);
	bool IsShiftDown();
	bool IsControlDown();
	bool IsAltDown();

	Vector2<int> GetMouseDelta();
}