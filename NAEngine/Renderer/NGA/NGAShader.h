#pragma once

#include "NGACommon.h"

namespace na
{
	enum class NGAShaderType
	{
		NONE = -1,
		VERTEX = 0,
		GEOMETRY,
		PIXEL
	};

	class NGAShader
	{
		NGA_GPU_CLASS(NGAShader);

	public:
		bool Construct(const char *filename, NGAShaderType type);
		void Destruct();

		bool IsConstructed()const;

		void* GetBytecode()const;
		size_t GetBytecodeSize()const;

		NGAShaderType GetType()const;

	private:
		NGAShaderType mType = NGAShaderType::NONE;

#if defined(NGA_D3D11)
	private:
		union {
			struct ID3D11VertexShader *mVertexShader;
			struct ID3D11GeometryShader *mGeometryShader;
			struct ID3D11PixelShader *mPixelShader;
		};

		struct ID3D10Blob *mBytecode;
#endif

		friend class NGACommandContext;
		friend class NGAPipelineState;
	};
}