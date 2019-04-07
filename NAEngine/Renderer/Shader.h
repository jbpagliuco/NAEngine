#pragma once

#include "Factory.h"
#include "InputLayout.h"
#include "ShaderProgram.h"

namespace na
{
	class Shader : public Factory<Shader>
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