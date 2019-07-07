#include "ImguiRenderer.h"

#include "RendererD3D.h"

#include "Vendor/imgui/imgui.h"

// I could rewrite this code, but what's the point?
#include "Vendor/imgui/examples/imgui_impl_win32.h"
#include "Vendor/imgui/examples/imgui_impl_dx11.h"

namespace na
{
	bool ImguiRendererSystemInit()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		if (!ImGui_ImplWin32_Init(NA_Renderer->GetWindow().handle)) {
			return false;
		}

		if (!ImGui_ImplDX11_Init(NA_RDevice, NA_RContext)) {
			return false;
		}

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
}