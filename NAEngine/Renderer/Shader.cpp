#include "Shader.h"

namespace na
{
	bool Shader::Initialize(InputLayout *inputLayout, VertexShader *vertexShader, PixelShader *pixelShader)
	{
		mVertexShader = vertexShader;
		mPixelShader = pixelShader;

		mInputLayout = inputLayout;

		return true;
	}

	void Shader::Shutdown()
	{
		mVertexShader->Shutdown();
		mPixelShader->Shutdown();

		mInputLayout->Shutdown();
	}

	void Shader::Bind()
	{
		mInputLayout->Bind();
		mVertexShader->Bind();
		mPixelShader->Bind();
	}
}