#include "NGA/NGAShader.h"

#if defined(NGA_D3D11)

#include <string>

#include <d3dcompiler.h>

#include "Base/File/File.h"
#include "Base/Memory/Memory.h"
#include "NGACoreInternalDX11.h"

namespace na
{
	NGA_GPU_CLASS_IMPLEMENT(NGAShader);

	static constexpr const char *SHADER_ENTRYPOINTS[] = {
		"vsMain",
		"psMain"
	};

	static constexpr const char *COMPILE_TARGETS[] = {
		"vs_5_0",
		"ps_5_0"
	};

	class ShaderProgramInclude : public ID3DInclude
	{
		virtual HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFilename, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes) override
		{
			std::string fullPath = "";
			switch(IncludeType) 
			{
			case D3D_INCLUDE_LOCAL:
			case D3D_INCLUDE_SYSTEM:
			{
				const bool found = FindFileRecursively(fullPath, "data\\shaders", pFilename);
				if (!found) {
					NA_ASSERT(false, "Failed to find HLSL include '%s'", pFilename);
					*ppData = nullptr;
					*pBytes = 0;
					return S_FALSE;
				}
				break;
			}

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

	static bool CompileShader(ID3D10Blob **outBuffer, const std::string &file, NGAShaderType type)
	{
		const char *entrypoint = SHADER_ENTRYPOINTS[(int)type];
		const char *target = COMPILE_TARGETS[(int)type];

		const size_t MAX_FILE_LENGTH = 256;
		wchar_t wfile[MAX_FILE_LENGTH];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, file.c_str(), -1, wfile, MAX_FILE_LENGTH);

		ShaderProgramInclude includer;

		ID3D10Blob *errorMessage = nullptr;
		HRESULT hr = D3DCompileFromFile(wfile, nullptr, &includer, entrypoint, target, D3D10_SHADER_ENABLE_STRICTNESS, 0, outBuffer, &errorMessage);
		if (FAILED(hr)) {
			if (errorMessage != nullptr) {
				NA_ASSERT(false, "Failed to compile HLSL shader '%s' with error message: %.*s", file.c_str(), errorMessage->GetBufferSize(), errorMessage->GetBufferPointer());
			}
			else {
				NA_ASSERT(false, "Failed to compile HLSL shader '%s' with unknown error", file.c_str());
			}
			return false;
		}

		return true;
	}



	bool NGAShader::Construct(const char *filename, NGAShaderType type)
	{
		NA_ASSERT_RETURN_VALUE(!IsConstructed(), false);

		const bool compiled = CompileShader(&mBytecode, filename, type);
		NA_ASSERT_RETURN_VALUE(compiled, false, "Failed to compile shader %s with target %s", filename, COMPILE_TARGETS[(int)type]);

		mType = type;

		HRESULT hr;
		switch (mType)
		{
		case NGAShaderType::VERTEX:
			hr = NgaDx11State.mDevice->CreateVertexShader(mBytecode->GetBufferPointer(), mBytecode->GetBufferSize(), nullptr, &mVertexShader);
			break;

		case NGAShaderType::PIXEL:
			hr = NgaDx11State.mDevice->CreatePixelShader(mBytecode->GetBufferPointer(), mBytecode->GetBufferSize(), nullptr, &mPixelShader);
			break;

		default:
			NA_ASSERT_RETURN_VALUE(false, false, "Unknown shader type (%d)", (int)type);
		}

		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to create shader %s", filename);

		return true;
	}

	void NGAShader::Destruct()
	{
		switch (mType)
		{
		case NGAShaderType::VERTEX:
			NGA_DX11_SAFE_RELEASE(mVertexShader);
			break;

		case NGAShaderType::PIXEL:
			NGA_DX11_SAFE_RELEASE(mPixelShader);
			break;
		}
	}

	bool NGAShader::IsConstructed()const
	{
		return mVertexShader != nullptr;
	}

	void* NGAShader::GetBytecode()const
	{
		return mBytecode->GetBufferPointer();
	}
	
	size_t NGAShader::GetBytecodeSize()const
	{
		return mBytecode->GetBufferSize();
	}

	NGAShaderType NGAShader::GetType()const
	{
		return mType;
	}
}

#endif