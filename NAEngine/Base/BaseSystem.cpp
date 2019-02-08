#include "BaseSystem.h"

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