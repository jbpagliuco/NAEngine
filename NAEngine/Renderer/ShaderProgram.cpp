#include "ShaderProgram.h"

#if defined(NA_D3D11)
#include <d3dcompiler.h>
#include "RendererD3D.h"
#endif

#include "Base/File/File.h"
#include "Base/Memory/Memory.h"

#define VERTEX_SHADER_TARGET "vs_5_0"
#define PIXEL_SHADER_TARGET  "ps_5_0"

namespace na
{
#if defined(NA_D3D11)
	class ShaderProgramInclude : public ID3DInclude
	{
		virtual HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFilename, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes) override
		{
			std::string fullPath = "";
			switch(IncludeType)
			{
			case D3D_INCLUDE_LOCAL:
			case D3D_INCLUDE_SYSTEM:
				FindFileRecursively(fullPath, "data\\shaders", pFilename);
				break;

			default:
				break;
			}

			File file(fullPath, std::ios::in);
			if (!file) {
				*ppData = nullptr;
				*pBytes = 0;
				return S_FALSE;
			}

			const UINT fileSize = (UINT)file.GetFileSize();

			if(fileSize > 0) {
				char *buf = (char*)NA_ALLOC(fileSize);
				file.ReadBytes(buf, fileSize);

				*ppData = buf;
				*pBytes = fileSize;
			} else {
				*ppData = nullptr;
				*pBytes = 0;
			}
			return S_OK;
		}

		virtual HRESULT __stdcall Close(LPCVOID pData) override
		{
			// Here we must correctly free buffer created in Open.
			NA_FREE((void*)pData);
			return S_OK;
		}

	};
#endif



#if defined(NA_D3D11)
	static bool CompileShader(ID3D10Blob **outBuffer, const std::string &file, const std::string &target)
	{
		const size_t MAX_FILE_LENGTH = 256;
		wchar_t wfile[MAX_FILE_LENGTH];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, file.c_str(), -1, wfile, MAX_FILE_LENGTH);

		ShaderProgramInclude includer;

		ID3D10Blob *errorMessage = nullptr;
		HRESULT hr = D3DCompileFromFile(wfile, nullptr, &includer, "main", target.c_str(), D3D10_SHADER_ENABLE_STRICTNESS, 0, outBuffer, &errorMessage);
		if (FAILED(hr)) {
			if (errorMessage != nullptr) {
				NA_ASSERT(false, "Failed to compile HLSL shader '%s' with error message: %.*s", file.c_str(), errorMessage->GetBufferSize(), errorMessage->GetBufferPointer());
			} else {
				NA_ASSERT(false, "Failed to compile HLSL shader '%s' with unknown error", file.c_str());
			}
			return false;
		}

		return true;
	}
#endif

	// Vertex Shader ////////////////////////////////////////////////////
	bool VertexShader::Initialize(const std::string &filename, size_t constantBufferSize)
	{
		if (!ShaderProgram::Initialize(filename, constantBufferSize)) {
			return false;
		}

#if defined(NA_D3D11)
		if (!CompileShader(&mBytecode, filename, VERTEX_SHADER_TARGET)) {
			return false;
		}

		HRESULT hr = NA_RDevice->CreateVertexShader(mBytecode->GetBufferPointer(), mBytecode->GetBufferSize(), nullptr, &mShader);
		if (FAILED(hr)) {
			return false;
		}
#endif

		return true;
	}

	void VertexShader::Bind()
	{
#if defined(NA_D3D11)
		NA_RContext->VSSetShader(mShader, nullptr, 0);

		if (mCB != nullptr) {
			NA_RContext->VSSetConstantBuffers(NA_RStateData->GetUserVSConstantBufferIndex(), 1, &mCB);
		}
#endif
	}

	void VertexShader::SetConstantBuffer(void *data, size_t size)
	{
#if defined(NA_D3D11)
		D3D11_MAPPED_SUBRESOURCE res;
		HRESULT hr = NA_RContext->Map(mCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
		NA_ASSERT_RETURN(SUCCEEDED(hr));

		memcpy(res.pData, data, size);
		NA_RContext->Unmap(mCB, 0);
#endif
	}



	// Pixel Shader ////////////////////////////////////////////////////
	bool PixelShader::Initialize(const std::string &filename, size_t constantBufferSize)
	{
		if (!ShaderProgram::Initialize(filename, constantBufferSize)) {
			return false;
		}

#if defined(NA_D3D11)
		if (!CompileShader(&mBytecode, filename, PIXEL_SHADER_TARGET)) {
			return false;
		}

		HRESULT hr = NA_RDevice->CreatePixelShader(mBytecode->GetBufferPointer(), mBytecode->GetBufferSize(), nullptr, &mShader);
		if (FAILED(hr)) {
			return false;
		}
#endif


		mCBSize = constantBufferSize;

		return true;
	}

	void PixelShader::Bind()
	{
#if defined(NA_D3D11)
		NA_RContext->PSSetShader(mShader, nullptr, 0);

		if (mCB != nullptr) {
			NA_RContext->PSSetConstantBuffers(NA_RStateData->GetUserPSConstantBufferIndex(), 1, &mCB);
		}
#endif
	}

	void PixelShader::SetConstantBuffer(void *data, size_t size)
	{
#if defined(NA_D3D11)
		D3D11_MAPPED_SUBRESOURCE res;
		HRESULT hr = NA_RContext->Map(mCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
		NA_ASSERT_RETURN(SUCCEEDED(hr));

		memcpy(res.pData, data, size);
		NA_RContext->Unmap(mCB, 0);
#endif
	}
}