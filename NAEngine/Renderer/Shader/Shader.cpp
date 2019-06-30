#include "Shader.h"

namespace na
{
	NA_FACTORY_SETUP(Shader);

	bool Shader::Initialize(VertexShader vertexShader, PixelShader pixelShader, InputLayout inputLayout)
	{
		mVertexShader = vertexShader;
		mPixelShader = pixelShader;
		mInputLayout = inputLayout;

		return true;
	}

	void Shader::Shutdown()
	{
		mVertexShader.Shutdown();
		mPixelShader.Shutdown();
		mInputLayout.Shutdown();
	}

	void Shader::Bind()
	{
		mInputLayout.Bind();
		mVertexShader.Bind();
		mPixelShader.Bind();
	}
}