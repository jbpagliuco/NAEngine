#pragma once

#include <string>

#include "Renderer/NGA/NGAShader.h"
#include "Renderer/NGA/NGACommon.h"

namespace na
{
	class ShaderProgram
	{
	public:
		virtual void Shutdown();

		const NGAShader& GetShader()const;

		virtual NGAShaderStage GetStage()const = 0;
		
	protected:
		NGAShader mShader;
	};

	template <NGAShaderStage STAGE>
	class ShaderProgramTemplate : public ShaderProgram
	{
	public:
		virtual NGAShaderStage GetStage()const override { return STAGE; }
	};

	class VertexShader : public ShaderProgramTemplate<NGA_SHADER_STAGE_VERTEX>
	{
	public:
		virtual bool Initialize(const std::string &filename, NGAVertexFormatDesc vertexFormatDesc);

		NGAVertexFormatDesc GetVertexFormatDesc()const;

	private:
		NGAVertexFormatDesc mVertexFormatDesc;
	};

	class GeometryShader : public ShaderProgramTemplate<NGA_SHADER_STAGE_GEOMETRY>
	{
	public:
		virtual bool Initialize(const std::string &filename);
	};

	class PixelShader : public ShaderProgramTemplate<NGA_SHADER_STAGE_PIXEL>
	{
	public:
		virtual bool Initialize(const std::string &filename);
	};
}