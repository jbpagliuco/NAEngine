#pragma once

#include "NGACommon.h"

namespace na
{
	class NGATexture;
	class NGASwapChain;

	class NGAShaderResourceView
	{
		NGA_GPU_CLASS(NGAShaderResourceView);

	public:
		bool Construct(const NGATexture &texture);
		void Destruct();

		bool IsConstructed()const;

#if defined(NGA_D3D11)
	private:
		struct ID3D11ShaderResourceView *mView;
#endif

		friend class NGACommandContext;
	};


	class NGARenderTargetView
	{
		NGA_GPU_CLASS(NGARenderTargetView);

	public:
		bool Construct(const NGATexture &texture);
		bool Construct(const NGASwapChain &swapChain);
		void Destruct();

		bool IsConstructed()const;

#if defined(NGA_D3D11)
	private:
		struct ID3D11RenderTargetView *mView;
#endif

		friend class NGACommandContext;
	};


	struct NGADepthStencilViewDesc
	{
		NGADepthBufferFormat mFormat;
		int mWidth;
		int mHeight;
	};

	class NGADepthStencilView
	{
		NGA_GPU_CLASS(NGADepthStencilView);

	public:
		bool Construct(const NGADepthStencilViewDesc &desc);
		void Destruct();

		bool IsConstructed()const;

#if defined(NGA_D3D11)
	private:
		struct ID3D11Texture2D *mBuffer;
		struct ID3D11DepthStencilView *mView;
#endif

		friend class NGACommandContext;
	};
}