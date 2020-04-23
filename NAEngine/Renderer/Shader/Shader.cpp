#include "Shader.h"

#include "Renderer/Renderer.h"

namespace na
{
	NA_FACTORY_SETUP(Shader);

	bool Shader::Initialize(const std::string &file, NGAVertexFormatDesc vsFormat)
	{
		mVertexShader.Initialize(file, vsFormat);
		mPixelShader.Initialize(file);

		return true;
	}

	void Shader::Shutdown()
	{
		mVertexShader.Shutdown();
		mPixelShader.Shutdown();
	}

	void Shader::Bind()
	{
		NA_RStateManager->BindShader(mVertexShader);
		NA_RStateManager->BindShader(mPixelShader);
	}
}