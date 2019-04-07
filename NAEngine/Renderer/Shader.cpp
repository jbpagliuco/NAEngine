#include "Shader.h"

namespace na
{
	NA_FACTORY_SETUP(Shader);

	bool Shader::Initialize(InputLayout *inputLayout, VertexShader *vertexShader, PixelShader *pixelShader)
	{
		mVertexShader = vertexShader;
		mPixelShader = pixelShader;

		mInputLayout = inputLayout;

		return true;
	}

	void Shader::Shutdown()
	{
		VertexShader::Destroy(mVertexShader);
		PixelShader::Destroy(mPixelShader);
		InputLayout::Destroy(mInputLayout);
	}

	void Shader::Bind()
	{
		mInputLayout->Bind();
		mVertexShader->Bind();
		mPixelShader->Bind();
	}
}