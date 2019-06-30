#pragma once

#include "Renderer/RenderDefs.h"

#include <string>

#if defined(NA_D3D11)
#include <d3d11.h>
#include "Renderer/RendererD3D.h"
#endif

#include "Base/Util/Util.h"
#include "Renderer/VertexFormat.h"

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
		// virtual bool Initialize(const std::string &filename);
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
		virtual bool Initialize(const std::string &filename, VertexFormatDesc vertexFormatDesc);
		
		virtual void Bind();

		inline VertexFormatDesc GetVertexFormatDesc()const { return mVertexFormatDesc; }

	private:
		VertexFormatDesc mVertexFormatDesc;
	};

	class PixelShader : public ShaderProgram<PlatformPixelShader>
	{
	public:
		virtual bool Initialize(const std::string &filename);

		virtual void Bind();
	};
}