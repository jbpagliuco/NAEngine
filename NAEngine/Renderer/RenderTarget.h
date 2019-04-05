#pragma once

#include "RenderDefs.h"

#if defined(NA_D3D11)
#include <d3d11.h>
#endif

namespace na
{
#if defined(NA_D3D11)
	typedef ID3D11RenderTargetView PlatformRenderTargetView;
	typedef ID3D11DepthStencilView PlatformDepthStencilView;
	typedef ID3D11Texture2D PlatformTexture2D; // TODO: Move this out of here
#endif

	class RenderTarget
	{
	public:
		bool Initialize();
		void Shutdown();

		void Clear(const float *clearColor);
		void Bind();

	private:
		PlatformRenderTargetView *mRTView;
		PlatformTexture2D *mDSBuffer;
		PlatformDepthStencilView *mDSView;
	};
}