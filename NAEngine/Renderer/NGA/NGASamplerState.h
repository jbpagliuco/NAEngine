#pragma once

#include "NGACommon.h"

#include "Base/Util/Util.h"

namespace na
{
	struct NGASamplerStateDesc
	{
		NGATextureFilter mFilter = NGATextureFilter::MIN_MAG_MIP_LINEAR;

		NGATextureAddressMode mAddressU = NGATextureAddressMode::WRAP;
		NGATextureAddressMode mAddressV = NGATextureAddressMode::WRAP;
		NGATextureAddressMode mAddressW = NGATextureAddressMode::WRAP;

		Tuple4f mBorderColor = Tuple4f(1.0f, 0.0f, 0.0f, 1.0f);
	};

	class NGASamplerState
	{
		NGA_GPU_CLASS(NGASamplerState);

	public:
		bool Construct(const NGASamplerStateDesc &desc);
		void Destruct();

		bool IsConstructed()const;

#if defined(NGA_D3D11)
	private:
		struct ID3D11SamplerState *mSamplerState;
#endif

		friend class NGACommandContext;
	};
}