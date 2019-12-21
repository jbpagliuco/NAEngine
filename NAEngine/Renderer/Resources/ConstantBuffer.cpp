#include "ConstantBuffer.h"

#include "Base/Util/Util.h"
#include "Renderer/Renderer.h"

namespace na
{
	bool ConstantBuffer::Initialize(ConstantBufferUsage usage, void *initialData, size_t size)
	{
		NA_ASSERT_RETURN_VALUE(!mBuffer.IsConstructed(), false);
		
		NGABufferDesc desc;
		desc.mUsage = NGA_BUFFER_USAGE_CONSTANT;
		desc.mSizeInBytes = RoundToNearestMultiple((int)size, 16);

		switch (usage) {
		case ConstantBufferUsage::IMMUTABLE:
			NA_ASSERT_RETURN_VALUE(initialData != nullptr, false, "Immutable buffers needs data upon creation.");
			break;

		case ConstantBufferUsage::GPU_WRITE:
			desc.mUsage |= NGA_BUFFER_USAGE_GPU_WRITE;
			break;

		case ConstantBufferUsage::CPU_WRITE:
			desc.mUsage |= NGA_BUFFER_USAGE_CPU_WRITE;
			break;
		};

		mBuffer.Construct(desc, initialData);
		mUsage = usage;
		mSize = size;

		return true;
	}

	void ConstantBuffer::Shutdown()
	{
		mBuffer.Destruct();
	}

	void ConstantBuffer::Map(void *pData)
	{
		NA_ASSERT_RETURN(mUsage == ConstantBufferUsage::CPU_WRITE, "Trying to map data to a non-CPU writeable buffer.");
		NA_RStateManager->MapBufferData(mBuffer, pData);
	}

	const NGABuffer& ConstantBuffer::GetBuffer()const
	{
		return mBuffer;
	}

	size_t ConstantBuffer::GetSize()const
	{
		return mSize;
	}
}