#include "Shader.h"

#include "Renderer/Renderer.h"

namespace na
{
	NA_FACTORY_SETUP(Shader);

	bool Shader::Initialize(const std::string &file, NGAVertexFormatDesc vsFormat)
	{
		mVertexShader.Initialize(file, vsFormat, 2);
		mPixelShader.Initialize(file, 2);

		return true;
	}

	bool Shader::Initialize(const std::string &vs, NGAVertexFormatDesc vsFormat, const std::string &ps)
	{
		mVertexShader.Initialize(vs, vsFormat, 1);
		mPixelShader.Initialize(ps, 1);

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