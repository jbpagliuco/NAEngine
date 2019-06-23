#pragma once

#include "RenderDefs.h"

#if defined(NA_D3D11)
#include <d3d11.h>
#endif

#include <vector>

#include "Base/Streaming/Stream.h"

#include "Formats.h"

namespace na
{
#if defined(NA_D3D11)
	typedef ID3D11InputLayout PlatformInputLayout;
#endif

	class VertexShader;

	struct InputElement
	{
		std::string mSemantic;
		int mIndex;
		Format mFormat;
	};

	class InputLayout : public Factory<InputLayout>
	{
	public:
		bool Initialize(const std::vector<InputElement> &elems, const VertexShader &vertexShader);
		void Shutdown();

		void Bind();

	private:
		PlatformInputLayout *mInputLayout;
	};
}