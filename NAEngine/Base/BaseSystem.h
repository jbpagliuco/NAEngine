#pragma once

#define BASE_LOG_FILTER "Base"

namespace na
{
	bool BaseSystemInit();
	void BaseSystemShutdown();
	void BaseSystemDoFrame();

	void BaseSystemDebugRender();
}