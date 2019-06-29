#include "Input.h"

#include "Base/OS/OS.h"
#include "Base/Util/Util.h"
#include "Renderer/RendererD3D.h"

#if defined(_NA_WIN32)
#include "Base/OS/OSWin32.h"
#endif

#define MODIFIER_KEY(flag, state) (!(flag) || state)

namespace na
{
	static bool KeyStates[0xff];

	static Vector2<int> MouseDelta = { 0, 0 };

	static void OnKeyDownCallback(int keyCode)
	{
		KeyStates[keyCode] = true;
	}

	static void OnKeyUpCallback(int keyCode)
	{
		KeyStates[keyCode] = false;
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
			OnKeyDownCallback((int)wparam);
			return true;
		}

		case WM_KEYUP:
		{
			OnKeyUpCallback((int)wparam);
			return true;
		}

		case WM_INPUT:
		{
			UINT dwSize = sizeof(RAWINPUT);
			static BYTE lpb[sizeof(RAWINPUT)];

			GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
			RAWINPUT *raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEMOUSE) {
				OnMouseMoveCallback(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
			}

			return false;
		}
		};

		return false;
	}





	bool InputSystemInit()
	{
		for (int i = 0; i < 0xFF; ++i) {
			KeyStates[i] = false;
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

		const Window &window = NA_Renderer->GetWindow();
		if (window.IsActiveWindow()) {
			const na::Point windowPos = window.GetPosition();
			const na::Size windowSize = window.GetViewportSize();
			SetCursorPos(windowPos.x + windowSize.x / 2, windowPos.y + windowSize.y / 2);
		}
	}

	void InputSystemShutdown()
	{

	}

	bool IsKeyDown(int keyCode, bool ctrl, bool shift, bool alt)
	{
		return KeyStates[keyCode] &&
			MODIFIER_KEY(ctrl, KeyStates[VK_CONTROL]) &&
			MODIFIER_KEY(shift, KeyStates[VK_SHIFT]) &&
			MODIFIER_KEY(alt, KeyStates[VK_MENU]);
	}

	bool IsShiftDown()
	{
		return KeyStates[VK_SHIFT];
	}

	bool IsControlDown()
	{
		return KeyStates[VK_CONTROL];
	}

	bool IsAltDown()
	{
		return KeyStates[VK_MENU];
	}

	Vector2<int> GetMouseDelta()
	{
		return MouseDelta;
	}
}