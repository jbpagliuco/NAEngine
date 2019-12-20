#include "ImguiRenderer.h"

#include "Base/OS/OSWin32.h"

#include "Renderer.h"

#include "Vendor/imgui/imgui.h"

// I could rewrite this code, but what's the point?
#include "Vendor/imgui/examples/imgui_impl_win32.h"
#include "Vendor/imgui/examples/imgui_impl_dx11.h"

#include "NGA/DX11/NGACoreInternalDX11.h"

namespace na
{
	static bool StylePushed = false;
	static bool IsFocused = false;

	bool ImguiRendererWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (IsFocused && ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
			return true;
		}
		
		return false;
	}

	bool ImguiRendererSystemInit()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		if (!ImGui_ImplWin32_Init(NA_Renderer->GetWindow().handle)) {
			return false;
		}

		if (!ImGui_ImplDX11_Init(NgaDx11State.mDevice, NgaDx11State.mContext)) {
			return false;
		}

		RegisterWndProcCallback(ImguiRendererWndProc);

		ImguiRendererSetFocus(IsFocused);

		return true;
	}

	void ImguiRendererSystemShutdown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void ImguiRendererBeginFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void ImguiRendererEndFrame()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void ImguiRendererSetFocus(bool focus)
	{
		IsFocused = focus;

		if (StylePushed) {
			ImGui::PopStyleVar(ImGuiStyleVar_Alpha);
		}

		if (focus) {
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
		} else {
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
		}

		StylePushed = true;
	}

	bool ImguiRendererGetFocus()
	{
		return IsFocused;
	}
}