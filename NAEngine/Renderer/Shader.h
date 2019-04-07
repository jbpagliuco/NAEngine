#pragma once

#include "InputLayout.h"
#include "ShaderProgram.h"

namespace na
{
	class Shader : public Factory<Shader>
	{
	public:
		bool Initialize(AssetID vertexShader, AssetID pixelShader);
		void Shutdown();

		void VSSetBufferData(void *data, size_t size);
		void PSSetBufferData(void *data, size_t size);

		void Bind();

	private:
		VertexShader *mVertexShader;
		PixelShader *mPixelShader;
		
		InputLayout *mInputLayout;
	};
}