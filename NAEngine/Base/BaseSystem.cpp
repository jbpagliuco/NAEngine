#include "BaseSystem.h"

#include "Memory/Memory.h"

namespace na
{
	bool BaseSystemInit()
	{
		return false;
	}

	void BaseSystemShutdown()
	{
		MemorySystemShutdown();
	}
}