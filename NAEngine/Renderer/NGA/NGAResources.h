#pragma once

#include "NGACommon.h"

#include "Base/Util/BitUtil.h"

namespace na
{
	/////////////////////////////////////////////////////
	// Textures
	enum class NGATextureType
	{
		TEXTURE1D,
		TEXTURE2D,
		TEXTURE3D
	};

	struct NGATextureDesc
	{
		NGATextureType mType;
	};

	class NGATexture
	{
		NGA_GPU_CLASS(NGATexture);

	public:
		bool Construct(const char *filename);
		bool Construct(const NGATextureDesc &desc, void *initialData);
		void Destruct();

		bool IsConstructed()const;

		const NGATextureDesc& GetDesc()const;
		
	private:
		NGATextureDesc mDesc;

#if defined(NGA_D3D11)
	private:
		struct ID3D11Resource *mResource;
#endif

		friend class NGAShaderResourceView;
	};


	/////////////////////////////////////////////////////
	// Buffers
	enum NGABufferUsage
	{
		NGA_BUFFER_USAGE_NONE			= 0,
		NGA_BUFFER_USAGE_VERTEX			= 1 << 0,
		NGA_BUFFER_USAGE_INDEX			= 1 << 1,
		NGA_BUFFER_USAGE_CONSTANT		= 1 << 2
	};
	NA_DEFINE_ENUM_FLAG_OPERATORS(NGABufferUsage);

	struct NGABufferDesc
	{
		NGABufferUsage mUsage = NGA_BUFFER_USAGE_NONE;
		uint32_t mSizeInBytes = 0;
	};

	class NGABuffer
	{
		NGA_GPU_CLASS(NGABuffer);

	public:
		bool Construct(const NGABufferDesc &desc);
		bool Construct(const NGABufferDesc &desc, void *initialData);
		void Destruct();

		bool IsConstructed()const;

#if defined(NGA_D3D11)
	private:
		struct ID3D11Buffer *mBuffer;

		friend class NGACommandContext;
#endif
	};
}