#include "PhysicsPhysX.h"

#include "PxPhysicsAPI.h"

#include "Base/Debug/Assert.h"
#include "Base/Debug/Log.h"
#include "Base/Console/Console.h"
#include "Base/Memory/Memory.h"

#include "Physics.h"

#define NA_PX_SAFE_RELEASE(x) if ((x)) { (x)->release(); (x) = nullptr; }

namespace na
{
	////////////////////////////////
	// Classes
	////////////////////////////////
	class PhysicsAllocator : public physx::PxAllocatorCallback
	{
	public:
		virtual ~PhysicsAllocator() {}
		virtual void* allocate(size_t size, const char *typeName, const char *filename, int line) override;
		virtual void deallocate(void* ptr) override;

		inline size_t GetBytesAllocated()const { return mAllocated; }

	private:
#if defined(NA_TRACK_MEMORY)
		size_t mAllocated = 0;
#endif
	};

	class PhysicsErrorHandler : public physx::PxErrorCallback
	{
		virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;
	};

	////////////////////////////////
	// Variables
	////////////////////////////////
	static constexpr size_t PHYSX_MEM_ALIGN = 16;

	static PhysicsAllocator Allocator;
	static PhysicsErrorHandler ErrorHandler;

	static physx::PxFoundation *PXFoundation;
	static physx::PxPhysics *PXPhysics;


	////////////////////////////////
	// Console Functions
	////////////////////////////////
	CONSOLE_COMMAND(physics_debug)
	{
		ConsolePrintf("Memory consumed by PhysX: %zu bytes", Allocator.GetBytesAllocated());
	}

	////////////////////////////////
	// External Functions
	////////////////////////////////
	bool PhysicsPhysXInit()
	{
		PXFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, Allocator, ErrorHandler);
		NA_FATAL_ERROR(PXFoundation, "PxCreateFoundation failed!");

		PXPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *PXFoundation, physx::PxTolerancesScale(), true, nullptr);
		NA_FATAL_ERROR(PXPhysics, "PxCreatePhysics failed!");

		return true;
	}

	void PhysicsPhysXShutdown()
	{
		NA_PX_SAFE_RELEASE(PXPhysics);
		NA_PX_SAFE_RELEASE(PXFoundation);
	}


	////////////////////////////////
	// Implementations
	////////////////////////////////
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


	void PhysicsErrorHandler::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		NA_ASSERT(false, "PhysX Error:\n\n(Code %d)\nMessage: %s\n\nFilename: %s\nLine: %d", (int)code, message, file, line);
	}
}