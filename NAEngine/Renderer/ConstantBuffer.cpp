#include "ConstantBuffer.h"

namespace na
{
	bool ConstantBuffer::Initialize(BufferUsage usage, void *pData, size_t dataByteLength)
	{
		NA_ASSERT(dataByteLength > 0, "Must initialize constant buffer with a positive data byte length.");

		if (usage == BufferUsage::IMMUTABLE) {
			NA_ASSERT(pData != nullptr, "You must set constant buffer immutable data at the time of creation.");
		}

		// ViewProj buffer
		D3D11_BUFFER_DESC desc;
		desc.Usage = (D3D11_USAGE)usage;
		desc.ByteWidth = RoundToNearestMultiple((int)dataByteLength, 16);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		
		switch (usage) {
		case BufferUsage::DEFAULT:
		case BufferUsage::IMMUTABLE:
			desc.CPUAccessFlags = 0;
			break;

		case BufferUsage::DYNAMIC:
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;

		case BufferUsage::STAGING:
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			break;
		};

		HRESULT hr;
		if (pData != nullptr) {
			D3D11_SUBRESOURCE_DATA srData{};
			srData.pSysMem = pData;

			hr = NA_RDevice->CreateBuffer(&desc, &srData, &mBuffer);
		} else {
			hr = NA_RDevice->CreateBuffer(&desc, nullptr, &mBuffer);
		}

		return SUCCEEDED(hr);
	}

	void ConstantBuffer::Shutdown()
	{
		NA_SAFE_RELEASE(mBuffer);
	}

	bool ConstantBuffer::Map(void *pData, size_t dataByteLength)
	{
		NA_ASSERT_RETURN_VALUE(mUsage != BufferUsage::IMMUTABLE && mUsage != BufferUsage::DEFAULT, false, "Trying to map data to an immutable buffer.");

		D3D11_MAPPED_SUBRESOURCE res;

		HRESULT hr = NA_RContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false);

		memcpy(res.pData, pData, dataByteLength);

		NA_RContext->Unmap(mBuffer, 0);

		return true;
	}

	PlatformConstantBuffer* ConstantBuffer::GetBuffer()const
	{
		return mBuffer;
	}

	size_t ConstantBuffer::GetSize()const
	{
		return mSize;
	}
}