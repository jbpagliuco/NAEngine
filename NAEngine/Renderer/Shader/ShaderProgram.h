#pragma once

#include "Renderer/RenderDefs.h"

#include <string>

#if defined(NA_D3D11)
#include <d3d11.h>
#include "Renderer/RendererD3D.h"
#endif

#include "Base/Util/Util.h"

namespace na
{
#if defined(NA_D3D11)
	typedef ID3D11VertexShader PlatformVertexShader;
	typedef ID3D11PixelShader PlatformPixelShader;
	typedef ID3D11Buffer PlatformConstantBuffer;

	typedef ID3D10Blob PlatformBlob;
#endif

	template <typename T>
	class ShaderProgram
	{
	public:
		virtual bool Initialize(const std::string &filename, size_t constantBufferSize) = 0;
		virtual void Shutdown()
		{
			NA_SAFE_RELEASE(mCB);
			NA_SAFE_RELEASE(mBytecode);
			NA_SAFE_RELEASE(mShader);
		}

		virtual void* GetBuffer()const { return mBytecode->GetBufferPointer(); }
		virtual size_t GetBufferSize()const { return mBytecode->GetBufferSize(); }

		virtual void SetConstantBuffer(void *data, size_t size) = 0;

	protected:
		T *mShader;
		PlatformBlob *mBytecode;

		PlatformConstantBuffer *mCB = nullptr;
		size_t mCBSize;
	};

	class VertexShader : public ShaderProgram<PlatformVertexShader>
	{
	public:
		virtual bool Initialize(const std::string &filename, size_t constantBufferSize) override;

		virtual void SetConstantBuffer(void *data, size_t size) override;

		virtual void Bind();
	};

	class PixelShader : public ShaderProgram<PlatformPixelShader>
	{
	public:
		virtual bool Initialize(const std::string &filename, size_t constantBufferSize) override;

		virtual void SetConstantBuffer(void *data, size_t size) override;

		virtual void Bind();
	};








	template <typename T>
	bool ShaderProgram<T>::Initialize(const std::string &filename, size_t constantBufferSize)
	{
		if (constantBufferSize > 0) {
			D3D11_BUFFER_DESC desc;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = RoundToNearestMultiple((int)constantBufferSize, 16);
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			HRESULT hr = NA_RDevice->CreateBuffer(&desc, nullptr, &mCB);
			NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to shader program constant buffer");
		}

		return true;
	}
}