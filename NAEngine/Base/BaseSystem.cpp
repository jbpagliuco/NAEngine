#include "BaseSystem.h"

#include "OS/OS.h"
#include "Memory/Memory.h"

namespace na
{
	bool BaseSystemInit()
	{
		// TODO: Move into renderer code
		CreateAndShowWindow(100, 100, 800, 600, L"NA Game LUL");

		return true;
	}

	void BaseSystemShutdown()
	{
		MemorySystemShutdown();
	}
}