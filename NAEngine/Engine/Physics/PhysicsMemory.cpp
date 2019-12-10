#include "PhysicsMemory.h"

namespace na
{
	static constexpr size_t PHYSX_MEM_ALIGN = 16;

	void* PhysicsAllocator::allocate(size_t size, const char *typeName, const char *filename, int line)
	{
#if defined(NA_TRACK_MEMORY)
		mAllocated += size;
#endif

		return AllocateAlignedMemory(size, PHYSX_MEM_ALIGN, filename, line, true, false);
	}

	void PhysicsAllocator::deallocate(void *ptr)
	{
#if defined(NA_TRACK_MEMORY)
		mAllocated -= GetAllocationSize(ptr);
#endif

		NA_FREE_ALIGNED(ptr);
	}
}