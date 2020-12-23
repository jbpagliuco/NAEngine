#include "Shader.h"

#include "Renderer/Renderer.h"

namespace na
{
	NA_FACTORY_SETUP(Shader);

	bool Shader::Initialize(const std::string &file, NGAVertexFormatDesc vsFormat, NGAShaderStage stages)
	{
		mVertexShader.Initialize(file, vsFormat);
		mPixelShader.Initialize(file);
		
		// Optional stages
		if (stages & NGA_SHADER_STAGE_GEOMETRY) {
			mGeometryShader.Initialize(file);
		}

		return true;
	}

	void Shader::Shutdown()
	{
		mVertexShader.Shutdown();
		mGeometryShader.Shutdown();
		mPixelShader.Shutdown();
	}

	void Shader::Bind()
	{
		NA_RStateManager->BindShader(*this);
	}
}