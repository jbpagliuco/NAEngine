#include "Shader.h"

namespace na
{
	NA_FACTORY_SETUP(Shader);

	bool Shader::Initialize(VertexShader vertexShader, PixelShader pixelShader)
	{
		mVertexShader = vertexShader;
		mPixelShader = pixelShader;

		return true;
	}

	void Shader::Shutdown()
	{
		mVertexShader.Shutdown();
		mPixelShader.Shutdown();
	}

	void Shader::Bind()
	{
		mVertexShader.Bind();
		mPixelShader.Bind();
	}
}