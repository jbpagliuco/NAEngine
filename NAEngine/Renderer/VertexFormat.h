#pragma once

#include <string>
#include <vector>

#include "Formats.h"

namespace na
{
	enum class VertexSemanticType {
		POSITION,
		NORMAL,
		TEXCOORD,
		BINORMAL,
		TANGENT,
		COLOR,
		
		UNKNOWN
	};

	struct VertexAttribute {
		Format mFormat;
		VertexSemanticType mSemanticType;
		int mSemanticIndex;
		int mOffset;
	};

	struct VertexFormatDesc {
		std::vector<VertexAttribute> mAttributes;
	};

	const char* GetSemanticName(VertexSemanticType type);
	VertexSemanticType GetSemanticType(const std::string &name);
}