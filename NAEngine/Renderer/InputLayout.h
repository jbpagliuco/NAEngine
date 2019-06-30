#pragma once

#include "RenderDefs.h"

#if defined(NA_D3D11)
#include <d3d11.h>
#endif

#include <vector>

#include "Formats.h"
#include "VertexFormat.h"

namespace na
{
#if defined(NA_D3D11)
	typedef ID3D11InputLayout PlatformInputLayout;
#endif

	class VertexShader;

	

	class InputLayout
	{
	public:
		bool Initialize(const VertexFormatDesc &vertexFormatDesc, const VertexShader &vertexShader);
		void Shutdown();

		void Bind();

	private:
		PlatformInputLayout *mInputLayout;
	};
}