#pragma once

#include <string>

#include "Renderer/NGA/NGAResources.h"
#include "Renderer/NGA/NGAResourceViews.h"
#include "Renderer/NGA/NGASamplerState.h"

namespace na
{
	struct RenderTargetDesc
	{
		int mWidth;
		int mHeight;

		NGADepthBufferFormat mDepthBufferFormat;
	};

	class RenderTarget
	{
	public:
		// Creates a render target to the back buffer.
		bool Initialize(const RenderTargetDesc &desc);
		// Creates a rende target to a texture.
		bool Initialize(const RenderTargetDesc &desc, const NGASamplerStateDesc &samplerStateDesc);

		void Shutdown();

		const NGATexture& GetResource()const;
		const NGASamplerState& GetSamplerState()const;

		const NGARenderTargetView& GetRenderTargetView()const;
		const NGADepthStencilView& GetDepthStencilView()const;
		const NGAShaderResourceView& GetShaderResourceView()const;

	private:
		NGATexture mTexture;
		NGASamplerState mSampler;

		NGARenderTargetView mRenderTargetView;
		NGADepthStencilView mDepthStencilView;
		NGAShaderResourceView mShaderResourceView;

		friend class StateManager;
	};
}