#include "Shader.h"

namespace na
{
	NA_FACTORY_SETUP(Shader);

	bool Shader::Initialize(AssetID vertexShader, AssetID pixelShader)
	{
		mVertexShader = VertexShader::Get(vertexShader);
		mVertexShader->AddRef();

		mPixelShader = PixelShader::Get(pixelShader);
		mPixelShader->AddRef();

		mInputLayout = InputLayout::Get(vertexShader);
		mInputLayout->AddRef();

		return true;
	}

	void Shader::Shutdown()
	{
		VertexShader::Destroy(mVertexShader);
		PixelShader::Destroy(mPixelShader);
		InputLayout::Destroy(mInputLayout);
	}

	void Shader::VSSetBufferData(void *data, size_t size)
	{
		mVertexShader->SetConstantBuffer(data, size);
	}

	void Shader::PSSetBufferData(void *data, size_t size)
	{
		mPixelShader->SetConstantBuffer(data, size);
	}

	void Shader::Bind()
	{
		mInputLayout->Bind();
		mVertexShader->Bind();
		mPixelShader->Bind();
	}
}