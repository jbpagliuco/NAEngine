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
		bool Initialize(const RenderTargetDesc &desc, const NGASamplerStateDesc &samplerStateDesc);
		void Shutdown();

		const NGATexture& GetResource()const;
		const NGASamplerState& GetSamplerState()const;

		const NGARenderTargetView& GetRenderTargetView()const;
		const NGAShaderResourceView& GetShaderResourceView()const;

	private:
		NGATexture mTexture;
		NGASamplerState mSampler;

		NGARenderTargetView mRTV;
		NGAShaderResourceView mSRV;
		NGADepthStencilView mDSV;

		friend class StateManager;
	};
}