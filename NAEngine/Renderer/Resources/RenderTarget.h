#pragma once

#include "Renderer/RenderDefs.h"

#if defined(NA_D3D)
struct ID3D11RenderTargetView;
#endif

namespace na
{
	class RenderTarget
	{
	public:
		bool Initialize();
		void Shutdown();

		void Clear(const float *clearColor);
		void Bind();

	private:

#if defined(NA_D3D)
		ID3D11RenderTargetView *mRTView;
#endif

	};
}