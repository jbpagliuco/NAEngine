#pragma once

#include "Base/Util/Util.h"

namespace na
{
	bool InputSystemInit();
	void InputSystemDoFrameLate();
	void InputSystemShutdown();
	
	bool IsKeyDown(int keyCode, bool ctrl = false, bool shift = false, bool alt = false);

	// Always returns key state, regardless of any special control filtering.
	bool IsSystemKeyDown(int keyCode, bool ctrl = false, bool shift = false, bool alt = false);
	bool IsSystemKeyPressed(int keyCode);
	bool IsSystemKeyReleased(int keyCode);

	bool IsShiftDown();
	bool IsControlDown();
	bool IsAltDown();

	Tuple2<int> GetMouseDelta();

	void ForceShowCursor(bool show);
}