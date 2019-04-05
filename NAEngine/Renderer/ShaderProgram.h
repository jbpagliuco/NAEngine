#pragma once

#include "RenderDefs.h"

#if defined(NA_D3D11)
#include <d3d11.h>
#endif

namespace na
{
#if defined(NA_D3D11)
	typedef ID3D11VertexShader PlatformVertexShader;
	typedef ID3D11PixelShader PlatformPixelShader;

	typedef ID3D10Blob PlatformBlob;
#endif

	template <typename T>
	class ShaderProgram
	{
	public:
		virtual bool Initialize(const char *filename) = 0;
		virtual void Shutdown()
		{
			NA_SAFE_RELEASE(mBytecode);
			NA_SAFE_RELEASE(mShader);
		}

		virtual void* GetBuffer()const { return mBytecode->GetBufferPointer(); }
		virtual size_t GetBufferSize()const { return mBytecode->GetBufferSize(); }

	protected:
		T *mShader;
		PlatformBlob *mBytecode;
	};

	class VertexShader : public ShaderProgram<PlatformVertexShader>
	{
	public:
		virtual bool Initialize(const char *filename);

		virtual void Bind();
	};

	class PixelShader : public ShaderProgram<PlatformPixelShader>
	{
	public:
		virtual bool Initialize(const char *filename);

		virtual void Bind();
	};
}