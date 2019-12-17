#pragma once

#include <stdint.h>

#include "NGADefs.h"

#if defined(NGA_D3D11)
#include <d3d11.h>
#endif

#include "Base/Debug/Assert.h"
#include "Base/Util/BitUtil.h"

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

	enum class NGATextureFilter {
#if defined(NGA_D3D11)
		MIN_MAG_MIP_POINT = D3D11_FILTER_MIN_MAG_MIP_POINT,
		MIN_MAG_POINT_MIP_LINEAR = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
		MIN_POINT_MAG_LINEAR_MIP_POINT = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
		MIN_LINEAR_MAG_MIP_POINT = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,

		MIN_MAG_MIP_LINEAR = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		MIN_MAG_LINEAR_MIP_POINT = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
		MIN_LINEAR_MAG_POINT_MIP_LINEAR = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		MIN_POINT_MAG_MIP_LINEAR = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR,

		ANISOTROPIC = D3D11_FILTER_ANISOTROPIC
#endif
	};
	
	enum NGAShaderStage {
		NGA_SHADER_STAGE_VERTEX		= 1 << 0,
		NGA_SHADER_STAGE_PIXEL		= 1 << 1,
		NGA_SHADER_STAGE_ALL		= 0x3 // Keep up to date with above
	};
	NA_DEFINE_ENUM_FLAG_OPERATORS(NGAShaderStage);

	enum class NGATextureAddressMode {
#if defined(NGA_D3D11)
		WRAP = D3D11_TEXTURE_ADDRESS_WRAP,
		MIRROR = D3D11_TEXTURE_ADDRESS_MIRROR,
		CLAMP = D3D11_TEXTURE_ADDRESS_CLAMP,
		BORDER = D3D11_TEXTURE_ADDRESS_BORDER,
		MIRROR_ONCE = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
#endif
	};
}