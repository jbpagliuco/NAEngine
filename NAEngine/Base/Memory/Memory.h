#pragma once

// Alignment
#ifndef ALIGN_MS
#if _MSC_VER && !__INTEL_COMPILER
#define ALIGN_MS(alignment) __declspec(align(alignment))
#else
#define ALIGN_MS(alignment)
#endif
#endif

#ifndef ALIGN_GCC
#if __GNUC__
#define ALIGN_GCC(alignment) __attribute__((aligned(alignment)))
#else
#define ALIGN_GCC(alignment)
#endif
#endif

#if defined(_NA_DEBUG)
#define NA_TRACK_MEMORY
#endif


#define NA_ALLOC(size) na::AllocateUnalignedMemory(size, __FILE__, __LINE__)
#define NA_ALLOC_ALIGNED(size, alignment) na::AllocateAlignedMemory(size, alignment, __FILE__, __LINE__)

#define NA_FREE(p) na::FreeUnalignedMemory(p);
#define NA_FREE_ALIGNED(p) na::FreeAlignedMemory(p);

namespace na
{
	void MemorySystemShutdown();

	// Allocates memory on the heap.
	// @param sizeInBytes - The number of bytes to allocate.
	// @return A pointer to the beginning of the memory.
	void * AllocateUnalignedMemory(size_t sizeInBytes, const char *filename, int line, bool track = true, bool staticMemory = false);

	// Frees memory allocated by AllocateUnalignedMemory.
	// @param pMem - A pointer returned by AllocateUnalignedMemory.
	void FreeUnalignedMemory(void * pMem);

	// Allocates memory on the heap with an address alignment.
	// @param sizeInBytes - The number of bytes to allocate.
	// @param alignment - The alignment value, must be a power of two.
	// @return A pointer to the beginning of the aligned memory.
	void * AllocateAlignedMemory(size_t sizeInBytes, size_t alignment, const char *filename, int line, bool track = true, bool staticMemory = false);

	// Frees memory allocated by AllocateAlignedMemory.
	// @param pMem - A pointer returned by AllocateAlignedMemory.
	void FreeAlignedMemory(void * pMem);

#if defined(NA_TRACK_MEMORY)
	size_t GetAllocationSize(void *pMem);
#endif
}