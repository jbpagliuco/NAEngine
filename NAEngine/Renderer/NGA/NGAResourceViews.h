#pragma once

#include "NGACommon.h"

namespace na
{
	class NGATexture;

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
}