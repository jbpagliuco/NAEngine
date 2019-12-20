#pragma once

#include "NGACommon.h"

#include "Base/OS/OS.h"

namespace na
{

	struct NGASwapChainDesc
	{
		Window mWindow = INVALID_WINDOW;
		int mBufferCount = 2;
	};

	class NGASwapChain
	{
		NGA_GPU_CLASS(NGASwapChain);

	public:
		bool Construct(const NGASwapChainDesc &desc);
		void Destruct();

		bool IsConstructed()const;

		void SetFullscreenState(bool fullscreen)const;
		void Present()const;

#if defined(NGA_D3D11)
	private:
		struct IDXGISwapChain *mSwapChain;
#endif

		// TODO: Might be a better way to handle this
		friend class NGARenderTargetView;
	};
}