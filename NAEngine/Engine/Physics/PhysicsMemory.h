#pragma once

#include "PxPhysicsAPI.h"

#include "Base/Memory/Memory.h"

namespace na
{
	class PhysicsAllocator : public physx::PxAllocatorCallback
	{
	public:
		virtual ~PhysicsAllocator() {}
		virtual void* allocate(size_t size, const char *typeName, const char *filename, int line) override;
		virtual void deallocate(void* ptr) override;

#if defined(NA_TRACK_MEMORY)
		inline size_t GetBytesAllocated()const { return mAllocated; }
	private:
		size_t mAllocated = 0;
#endif
	};
}