#pragma once

namespace na
{
	bool ImguiRendererSystemInit();
	void ImguiRendererSystemShutdown();

	void ImguiRendererBeginFrame();
	void ImguiRendererEndFrame();
}