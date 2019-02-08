#include "Memory.h"

#include "Debug/Assert.h"
#include "Debug/Log.h"

#define MEMORY_LOG_FILTER "Memory"

#if defined(_NA_DEBUG)
#define NA_TRACK_MEMORY
#endif

#if defined(NA_TRACK_MEMORY)
#include <map>
struct MemoryAllocation
{
	size_t size;
	const char *filename;
	int line;
};

static std::map<void*, MemoryAllocation> MemoryAllocations;
static size_t TotalBytesAllocated = 0;
static size_t TotalBytesAllocatedExtra = 0;
#endif


namespace na
{
	void MemorySystemShutdown()
	{
#if defined(NA_TRACK_MEMORY)
		for (auto &alloc : MemoryAllocations) {
			LogError(MEMORY_LOG_FILTER, "Allocation of '%zu' bytes from '%s line %d' was never freed.", alloc.second.size, alloc.second.filename, alloc.second.line);
		}
		NA_ASSERT_RETURN(MemoryAllocations.size() == 0, "Memory was leaked. Check logs.");

		NA_ASSERT_RETURN(TotalBytesAllocated == 0);
		NA_ASSERT_RETURN(TotalBytesAllocatedExtra == 0);
#endif
	}

	void * AllocateUnalignedMemory(size_t sizeInBytes, const char *filename, int line)
	{
		void *p = new unsigned char[sizeInBytes];
#if defined(NA_TRACK_MEMORY)
		MemoryAllocations[p] = { sizeInBytes, filename, line };
		TotalBytesAllocated += sizeInBytes;
#endif
		return p;
	}

	void FreeUnalignedMemory(void * pMem)
	{
#if defined(NA_TRACK_MEMORY)
		TotalBytesAllocated -= MemoryAllocations[pMem].size;
		MemoryAllocations.erase(pMem);
#endif
		delete[] pMem;
	}



	void * AllocateAlignedMemory(size_t sizeInBytes, size_t alignment, const char *filename, int line)
	{
		NA_ASSERT_RETURN_VALUE(alignment >= 1, nullptr);
		NA_ASSERT_RETURN_VALUE(alignment <= 128, nullptr);
		NA_ASSERT_RETURN_VALUE((alignment & (alignment - 1)) == 0, nullptr); // Check to see if power of 2.

		size_t totalMemoryToAllocate = sizeInBytes + alignment;

		uintptr_t rawAddress = reinterpret_cast<uintptr_t>(AllocateUnalignedMemory(totalMemoryToAllocate, filename, line));

		size_t mask = (alignment - 1);
		uintptr_t offset = (rawAddress & mask);
		ptrdiff_t adjustment = alignment - offset;

		uintptr_t alignedAddress = rawAddress + adjustment;

		NA_ASSERT_RETURN_VALUE(adjustment < 256, nullptr);
		unsigned char * pAlignedMemory = reinterpret_cast<unsigned char*>(alignedAddress);
		pAlignedMemory[-1] = static_cast<unsigned char>(adjustment); // We will always have at least an extra 
																	 // byte before the aligned address if the
																	 // asserts pass.

#if defined(NA_TRACK_MEMORY)
		TotalBytesAllocatedExtra += alignment;
#endif
		return static_cast<void*>(pAlignedMemory);
	}

	void FreeAlignedMemory(void * pMem)
	{
		const unsigned char * pAlignedMem = reinterpret_cast<unsigned char*>(pMem);
		uintptr_t pAlignedAddress = reinterpret_cast<uintptr_t>(pMem);
		ptrdiff_t adjustment = static_cast<ptrdiff_t>(pAlignedMem[-1]);

		uintptr_t rawAddress = pAlignedAddress - adjustment;
		void * pRawMem = reinterpret_cast<void*>(rawAddress);

#if defined(NA_TRACK_MEMORY)
		ptrdiff_t extra = pAlignedAddress - rawAddress;
		TotalBytesAllocatedExtra -= extra;
#endif

		FreeUnalignedMemory(pRawMem);
	}
#pragma endregion
}