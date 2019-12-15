#pragma once

#include <stdint.h>

#include "NGADefs.h"

#if defined(NGA_D3D11)
#include <d3d11.h>
#endif

#include "Base/Debug/Assert.h"

#define NGA_GPU_CLASS(ClassName)								\
	public:														\
	ClassName() {}												\
	~ClassName() { Destruct(); }								\
	ClassName(const ClassName&) = delete;						\
	ClassName& operator=(const ClassName&) = delete;			\
	static const ClassName INVALID;								\
	private:

#define NGA_GPU_CLASS_IMPLEMENT(ClassName)						\
	const ClassName ClassName::INVALID

namespace na
{
	enum class NGAIndexBufferType
	{
		IBT_16BIT,
		IBT_32BIT
	};

	enum class NGAPrimitiveTopology
	{
#if defined(NGA_D3D11)
		TRIANGLE_LIST = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST	
#endif
	};
}