#include "Input.h"

#include "Base/OS/OS.h"
#include "Base/Util/Util.h"
#include "Renderer/ImguiRenderer.h"
#include "Renderer/Renderer.h"

#if defined(_NA_WIN32)
#include "Base/OS/OSWin32.h"
#endif

#define MODIFIER_KEY(flag, state) (!(flag) || state)

namespace na
{
	struct KeyState {
		bool mDown = false;
		bool mPressed = false;
		bool mReleased = false;
	};
	static KeyState KeyStates[0xff];

	static Tuple2<int> MouseDelta = { 0, 0 };
	
	static bool InputEnabled = true;

	
	void ForceShowCursor(bool show)
	{
		if (show) {
			while (ShowCursor(true) < 0);
		} else {
			while (ShowCursor(false) >= 0);
		}
	}

	static void SetFocus(bool focus)
	{
		InputEnabled = focus;
		ForceShowCursor(!focus);
	}

	static void OnKeyDownCallback(int keyCode)
	{
		KeyStates[keyCode].mPressed = true;
		KeyStates[keyCode].mDown = true;
	}

	static void OnKeyUpCallback(int keyCode)
	{
		KeyStates[keyCode].mReleased = true;
		KeyStates[keyCode].mDown = false;
	}

	static void OnMouseMoveCallback(int dx, int dy)
	{
		MouseDelta = { dx, dy };
	}

	static bool InputWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg) {
		case WM_KEYDOWN:
		{
			if (InputEnabled) {
				OnKeyDownCallback((int)wparam);
				return true;
			}
		}

		case WM_KEYUP:
		{
			if (InputEnabled) {
				OnKeyUpCallback((int)wparam);
				return true;
			}
		}

		case WM_INPUT:
		{
			if (InputEnabled) {
				UINT dwSize = sizeof(RAWINPUT);
				static BYTE lpb[sizeof(RAWINPUT)];

				GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
				RAWINPUT *raw = (RAWINPUT*)lpb;

				if (raw->header.dwType == RIM_TYPEMOUSE) {
					OnMouseMoveCallback(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}

				return false;
			}
		}

		case WM_ACTIVATE:
			SetFocus(NA_Renderer->GetWindow().IsActiveWindow());
			return true;
		};

		return false;
	}





	bool InputSystemInit()
	{
		for (int i = 0; i < 0xFF; ++i) {
			KeyStates[i] = KeyState();
		}

		const USHORT HID_USAGE_PAGE_GENERIC = 0x01;
		const USHORT HID_USAGE_GENERIC_MOUSE = 0x02;

		RAWINPUTDEVICE rid[1];
		rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		rid[0].dwFlags = RIDEV_INPUTSINK;
		rid[0].hwndTarget = NA_Renderer->GetWindow().handle;
		const BOOL registered = RegisterRawInputDevices(rid, 1, sizeof(rid[0]));
		NA_ASSERT(registered, "Failed to register mouse input device.");

#if !defined(_NA_TOOLS)
		// Hide the mouse cursor
		while (ShowCursor(false) > 0);
#endif

		RegisterWndProcCallback(&InputWndProc);

		return true;
	}

	void InputSystemDoFrameLate()
	{
		MouseDelta = { 0, 0 };

		if (InputEnabled && !ImguiRendererGetFocus()) {
			const Window &window = NA_Renderer->GetWindow();
			const na::Point windowPos = window.GetPosition();
			const na::Size windowSize = window.GetViewportSize();
			SetCursorPos(windowPos.x + windowSize.x / 2, windowPos.y + windowSize.y / 2);
		}

		for (int i = 0; i < 0xFF; ++i) {
			KeyStates[i].mPressed = false;
			KeyStates[i].mReleased = false;
		}
	}

	void InputSystemShutdown()
	{

	}

	bool IsKeyDown(int keyCode, bool ctrl, bool shift, bool alt)
	{
		if (ImguiRendererGetFocus()) {
			return false;
		}

		return IsSystemKeyDown(keyCode, ctrl, shift, alt);
	}

	bool IsKeyPressed(int keyCode, bool ctrl, bool shift, bool alt)
	{
		if (ImguiRendererGetFocus()) {
			return false;
		}
		
		return IsSystemKeyPressed(keyCode);
	}

	bool IsSystemKeyDown(int keyCode, bool ctrl, bool shift, bool alt)
	{
		return KeyStates[keyCode].mDown &&
			MODIFIER_KEY(ctrl, KeyStates[VK_CONTROL].mDown) &&
			MODIFIER_KEY(shift, KeyStates[VK_SHIFT].mDown) &&
			MODIFIER_KEY(alt, KeyStates[VK_MENU].mDown);
	}


	bool IsSystemKeyPressed(int keyCode, bool ctrl, bool shift, bool alt)
	{
		return KeyStates[keyCode].mPressed &&
			MODIFIER_KEY(ctrl, KeyStates[VK_CONTROL].mDown) &&
			MODIFIER_KEY(shift, KeyStates[VK_SHIFT].mDown) &&
			MODIFIER_KEY(alt, KeyStates[VK_MENU].mDown);
	}

	bool IsSystemKeyReleased(int keyCode, bool ctrl, bool shift, bool alt)
	{
		return KeyStates[keyCode].mReleased &&
			MODIFIER_KEY(ctrl, KeyStates[VK_CONTROL].mDown) &&
			MODIFIER_KEY(shift, KeyStates[VK_SHIFT].mDown) &&
			MODIFIER_KEY(alt, KeyStates[VK_MENU].mDown);
	}

	bool IsShiftDown()
	{
		return KeyStates[VK_SHIFT].mDown;
	}

	bool IsControlDown()
	{
		return KeyStates[VK_CONTROL].mDown;
	}

	bool IsAltDown()
	{
		return KeyStates[VK_MENU].mDown;
	}

	Tuple2<int> GetMouseDelta()
	{
		if (ImguiRendererGetFocus()) {
			return Tuple2<int>(0, 0);
		}

		return MouseDelta;
	}
}