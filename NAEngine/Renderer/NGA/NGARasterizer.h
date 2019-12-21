#pragma once

#include "NGACommon.h"

namespace na
{
	enum class NGACullMode
	{
#if defined(NGA_D3D11)
		CULL_BACK = D3D11_CULL_BACK
#endif
	};

	enum class NGAFillMode
	{
#if defined(NGA_D3D11)
		SOLID = D3D11_FILL_SOLID
#endif
	};

	struct NGARasterizerStateDesc
	{
		bool mAntialias;
		NGACullMode mCullMode;
		NGAFillMode mFillMode;
		bool mFrontCounterClockwise;
	};

	class NGARasterizerState
	{
		NGA_GPU_CLASS(NGARasterizerState);

	public:
		bool Construct(const NGARasterizerStateDesc &desc);
		void Destruct();

		bool IsConstructed()const;

#if defined(NGA_D3D11)
	private:
		struct ID3D11RasterizerState *mRasterizerState;
#endif

		friend class NGACommandContext;
	};
}