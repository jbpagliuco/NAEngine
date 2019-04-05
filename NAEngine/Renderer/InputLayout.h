#pragma once

#include "RenderDefs.h"

#if defined(NA_D3D11)
#include <d3d11.h>
#endif

#include <vector>

#include "Formats.h"

namespace na
{
#if defined(NA_D3D11)
	typedef ID3D11InputLayout PlatformInputLayout;
#endif

	class VertexShader;

	struct InputElement
	{
		const char *mSemantic;
		int mIndex;
		Format mFormat;
	};

	class InputLayout
	{
	public:
		bool Initialize(const std::vector<InputElement> &elems, const VertexShader &vertexShader);
		void Shutdown();

		void Bind();

	private:
		PlatformInputLayout *mInputLayout;
	};
}