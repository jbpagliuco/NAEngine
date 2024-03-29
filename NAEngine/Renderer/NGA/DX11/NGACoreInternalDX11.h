#pragma once

#include "NGA/NGADefs.h"

#if defined(NGA_D3D11)

#include <d3d11.h>

#include "NGA/NGACommon.h"

#define NGA_DX11_SAFE_RELEASE(x) if ((x)) { (x)->Release(); (x) = nullptr; }

namespace na
{
	class NGAInternalStateDX11
	{
	public:
		ID3D11Device *mDevice;
		ID3D11DeviceContext *mContext;
	};

	DXGI_FORMAT NGAFormatToDXGI(NGAFormat format);
	DXGI_FORMAT NGATypelessFormatToColorDXGI(NGAFormat format);
	DXGI_FORMAT NGATypelessFormatToDepthDXGI(NGAFormat format);

	D3D11_USAGE NGAUsageToD3D11(NGAUsage usage);

	NGAFormat DXGIFormatToNGA(DXGI_FORMAT format);

	extern NGAInternalStateDX11 NgaDx11State;
}

#endif // defined(NGA_D3D11)