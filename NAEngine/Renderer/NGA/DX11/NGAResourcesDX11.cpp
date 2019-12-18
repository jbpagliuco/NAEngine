#include "NGA/NGAResources.h"

#if defined(NGA_D3D11)

#include "Vendor/DirectXTK/Include/DDSTextureLoader.h"

#include "Base/Util/String.h"

#include "NGACoreInternalDX11.h"

namespace na
{
	NGA_GPU_CLASS_IMPLEMENT(NGATexture);
	NGA_GPU_CLASS_IMPLEMENT(NGABuffer);


	bool NGATexture::Construct(const char *filename)
	{
		const HRESULT hr = DirectX::CreateDDSTextureFromFile(NgaDx11State.mDevice, ToWideString(filename).c_str(), &mResource, nullptr);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to load texture from file %s (HR: %X)", filename, hr);

		mDesc.mType = NGATextureType::TEXTURE2D;

		return true;
	}

	bool NGATexture::Construct(const NGATextureDesc &desc, void *initialData)
	{
		NA_ASSERT_RETURN_VALUE(false, false, "Unimplemented.");

		mDesc = desc;
	}

	void NGATexture::Destruct()
	{
		NGA_DX11_SAFE_RELEASE(mResource);
	}

	bool NGATexture::IsConstructed()const
	{
		return mResource != nullptr;
	}

	const NGATextureDesc& NGATexture::GetDesc()const
	{
		return mDesc;
	}



	D3D11_USAGE NgaDx11TranslateUsage(NGABufferUsage usage)
	{
		if (usage & NGA_BUFFER_USAGE_CPU_READ_WRITE) {
			return D3D11_USAGE_STAGING;
		}

		if (usage & NGA_BUFFER_USAGE_GPU_WRITE) {
			return D3D11_USAGE_DEFAULT;
		}

		if (usage & NGA_BUFFER_USAGE_CPU_WRITE) {
			return D3D11_USAGE_DYNAMIC;
		}

		return D3D11_USAGE_IMMUTABLE;
	}

	UINT NgaDx11TranslateBindUsage(NGABufferUsage usage)
	{
		UINT dxusage = 0;

		if (usage & NGA_BUFFER_USAGE_VERTEX) {
			dxusage |= D3D11_BIND_VERTEX_BUFFER;
		}
		if (usage & NGA_BUFFER_USAGE_INDEX) {
			dxusage |= D3D11_BIND_INDEX_BUFFER;
		}
		if (usage & NGA_BUFFER_USAGE_CONSTANT) {
			NA_ASSERT(dxusage == 0, "Constant buffers cannot be combined with any other binding type.");
			dxusage = D3D11_BIND_CONSTANT_BUFFER;
		}

		return dxusage;
	}

	UINT NgaDx11TranslateCPUAccessFlags(NGABufferUsage usage)
	{
		if (usage & NGA_BUFFER_USAGE_CPU_WRITE) {
			return D3D11_CPU_ACCESS_WRITE;
		}

		if (usage & NGA_BUFFER_USAGE_CPU_READ_WRITE) {
			return D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		}

		return 0;
	}

	bool NGABuffer::Construct(const NGABufferDesc &desc)
	{
		return Construct(desc, nullptr);
	}

	bool NGABuffer::Construct(const NGABufferDesc &desc, void *initialData)
	{
		NA_ASSERT_RETURN_VALUE(!IsConstructed(), false, "Buffer is already constructed.");

		D3D11_BUFFER_DESC dxDesc;
		dxDesc.Usage = NgaDx11TranslateUsage(desc.mUsage);
		dxDesc.BindFlags = NgaDx11TranslateBindUsage(desc.mUsage);
		dxDesc.ByteWidth = (UINT)desc.mSizeInBytes;
		dxDesc.CPUAccessFlags = NgaDx11TranslateCPUAccessFlags(desc.mUsage);
		dxDesc.MiscFlags = 0;
		dxDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA *pSubResData = nullptr;
		D3D11_SUBRESOURCE_DATA subResData;
		if (initialData != nullptr) {
			subResData.pSysMem = initialData;
			pSubResData = &subResData;
		}

		HRESULT hr = NgaDx11State.mDevice->CreateBuffer(&dxDesc, pSubResData, &mBuffer);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "ID3D11Device::CreateBuffer() failed with HRESULT %X", hr);

		mDesc = desc;

		return true;
	}

	void NGABuffer::Destruct()
	{
		NGA_DX11_SAFE_RELEASE(mBuffer);
	}

	bool NGABuffer::IsConstructed()const
	{
		return mBuffer != nullptr;
	}
}

#endif // defined(NGA_D3D11)