#pragma once

namespace na
{
	bool RenderingSystemInit();
	void RenderingSystemShutdown();

	void RenderingSystemBeginFrame();
	void RenderingSystemDoFrame();
	void RenderingSystemEndFrame();
}