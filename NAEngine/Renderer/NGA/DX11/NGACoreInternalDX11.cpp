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
		};

		return CONV[(int)format];
	}
}

#endif //d efined(NGA_D3D11)