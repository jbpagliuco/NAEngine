#include "Shader.h"

#include "Renderer/Renderer.h"

namespace na
{
	NA_FACTORY_SETUP(Shader);

	bool Shader::Initialize(const std::string &vs, NGAVertexFormatDesc vsFormat, const std::string &ps)
	{
		mVertexShader.Initialize(vs, vsFormat);
		mPixelShader.Initialize(ps);

		return true;
	}

	void Shader::Shutdown()
	{
		mVertexShader.Shutdown();
		mPixelShader.Shutdown();
	}

	void Shader::Bind()
	{
		NA_RStateData->BindShader(mVertexShader);
		NA_RStateData->BindShader(mPixelShader);
	}
}