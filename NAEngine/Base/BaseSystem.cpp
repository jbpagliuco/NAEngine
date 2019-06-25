#include "BaseSystem.h"

#include "Debug/Assert.h"
#include "Debug/Log.h"
#include "OS/OS.h"
#include "Memory/Memory.h"
#include "Streaming/Stream.h"

namespace na
{
	bool BaseSystemInit()
	{
#define SYSTEM_INIT(name, f) LogInfo(BASE_LOG_FILTER, "Initializing %s", name); if (!f()) { NA_ASSERT_RETURN_VALUE(false, false, "Failed to initialize %s", name); }

		SYSTEM_INIT("Streaming system", StreamSystemInit);

		return true;

#undef SYSTEM_INIT
	}

	void BaseSystemShutdown()
	{
		StreamSystemShutdown();
		MemorySystemShutdown();
	}
}