#pragma once

#include "ShaderProgram.h"
#include "InputLayout.h"

namespace na
{
	class Shader
	{
	public:
		bool Initialize(InputLayout *inputLayout, VertexShader *vertexShader, PixelShader *pixelShader);
		void Shutdown();

		void Bind();

	private:
		VertexShader *mVertexShader;
		PixelShader *mPixelShader;
		
		InputLayout *mInputLayout;
	};
}