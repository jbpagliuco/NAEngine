#include "NGACoreInternalDX11.h"

#if defined(NGA_D3D11)

namespace na
{
	NGAInternalStateDX11 NgaDx11State;

	DXGI_FORMAT NGAFormatToDXGI(NGAFormat format)
	{
		static constexpr DXGI_FORMAT CONV[] = {
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_R32G32B32_FLOAT,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		};

		return CONV[(int)format];
	}

	DXGI_FORMAT NGAFormatToDXGI(NGADepthBufferFormat format)
	{
		static constexpr DXGI_FORMAT CONV[] = {
			DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT_D16_UNORM,
			DXGI_FORMAT_D24_UNORM_S8_UINT
		};

		return CONV[(int)format];
	}

	D3D11_USAGE NGAUsageToD3D11(NGAUsage usage)
	{
		static constexpr D3D11_USAGE CONV[] = {
			D3D11_USAGE_IMMUTABLE,
			D3D11_USAGE_DEFAULT,
			D3D11_USAGE_DYNAMIC,
			D3D11_USAGE_STAGING
		};

		return CONV[(int)usage];
	}
}

#endif //d efined(NGA_D3D11)