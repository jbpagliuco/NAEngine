#pragma once

#include <vector>

#include "NGACommon.h"

namespace na
{
	class NGAShader;

	class NGAInputLayout
	{
		NGA_GPU_CLASS(NGAInputLayout);

	public:
		bool Construct(const NGAVertexFormatDesc &vertexFormatDesc, const NGAShader &vertexShader);
		void Destruct();

		bool IsConstructed()const;

#if defined(NGA_D3D11)
	private:
		struct ID3D11InputLayout *mInputLayout;
#endif

		friend class NGACommandContext;
	};
}