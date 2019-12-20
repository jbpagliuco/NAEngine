#pragma once

#include "Base/Streaming/AssetFactory.h"
#include "ShaderProgram.h"

namespace na
{
	class Shader : public AssetFactory<Shader>
	{
	public:
		bool Initialize(const std::string &vs, NGAVertexFormatDesc vsFormat, const std::string &ps);
		void Shutdown();

		void Bind();

		inline const VertexShader& GetVertexShader()const { return mVertexShader; }
		inline const PixelShader& GetPixelShader()const { return mPixelShader; }

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
	};
}