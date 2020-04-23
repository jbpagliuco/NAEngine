#include "ShaderProgram.h"

namespace na
{
	void ShaderProgram::Shutdown()
	{
		mShader.Destruct();
	}

	const NGAShader& ShaderProgram::GetShader()const
	{
		return mShader;
	}


	bool VertexShader::Initialize(const std::string &filename, NGAVertexFormatDesc vertexFormatDesc, int version)
	{
		mVertexFormatDesc = vertexFormatDesc;
		return mShader.Construct(filename.c_str(), NGAShaderType::VERTEX, version);
	}

	NGAVertexFormatDesc VertexShader::GetVertexFormatDesc()const
	{
		return mVertexFormatDesc;
	}


	bool PixelShader::Initialize(const std::string &filename, int version)
	{
		return mShader.Construct(filename.c_str(), NGAShaderType::PIXEL, version);
	}
}