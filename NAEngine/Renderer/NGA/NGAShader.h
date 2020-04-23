#pragma once

#include "NGACommon.h"

namespace na
{
	enum class NGAShaderType
	{
		VERTEX = 0,
		PIXEL
	};

	class NGAShader
	{
		NGA_GPU_CLASS(NGAShader);

	public:
		bool Construct(const char *filename, NGAShaderType type, int version);
		void Destruct();

		bool IsConstructed()const;

		void* GetBytecode()const;
		size_t GetBytecodeSize()const;

		NGAShaderType GetType()const;

	private:
		NGAShaderType mType;

#if defined(NGA_D3D11)
	private:
		union {
			struct ID3D11VertexShader *mVertexShader;
			struct ID3D11PixelShader *mPixelShader;
		};

		struct ID3D10Blob *mBytecode;
#endif

		friend class NGACommandContext;
		friend class NGAPipelineState;
	};
}