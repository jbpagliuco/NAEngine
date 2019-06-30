#pragma once

#include "Base/Streaming/AssetFactory.h"
#include "Renderer/InputLayout.h"
#include "ShaderProgram.h"

namespace na
{
	class Shader : public AssetFactory<Shader>
	{
	public:
		bool Initialize(VertexShader vertexShader, PixelShader pixelShader, InputLayout inputLayout);
		void Shutdown();

		void Bind();

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		
		InputLayout mInputLayout;
	};
}