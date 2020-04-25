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

		bool operator==(const NGAShaderResourceView& other)const;
		bool operator!=(const NGAShaderResourceView& other)const;

		void GenerateMips()const;

#if defined(NGA_D3D11)
	private:
		struct ID3D11ShaderResourceView *mView = nullptr;
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

		bool operator==(const NGARenderTargetView& other)const;

#if defined(NGA_D3D11)
	private:
		struct ID3D11RenderTargetView *mView = nullptr;
#endif

		friend class NGACommandContext;
	};


	struct NGADepthStencilViewDesc
	{
		NGAFormat mFormat;
		int mWidth;
		int mHeight;
	};

	class NGADepthStencilView
	{
		NGA_GPU_CLASS(NGADepthStencilView);

	public:
		bool Construct(const NGATexture &texture);
		void Destruct();

		bool IsConstructed()const;

		bool operator==(const NGADepthStencilView& other)const;

#if defined(NGA_D3D11)
	private:
		struct ID3D11DepthStencilView *mView = nullptr;
#endif

		friend class NGACommandContext;
	};
}