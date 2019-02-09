#include "BaseSystem.h"

#include "OS/OS.h"
#include "Memory/Memory.h"

namespace na
{
	bool BaseSystemInit()
	{
		return true;
	}

	void BaseSystemShutdown()
	{
		MemorySystemShutdown();
	}
}