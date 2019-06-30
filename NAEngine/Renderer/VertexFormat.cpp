#include "VertexFormat.h"

namespace na
{
	static std::string Semantics[] = {
		"POSITION",
		"NORMAL",
		"TEXCOORD",
		"BINORMAL",
		"TANGENT",
		"COLOR",

		"UNKNOWN"
	};

	const char* GetSemanticName(VertexSemanticType type)
	{
		return Semantics[(int)type].c_str();
	}

	VertexSemanticType GetSemanticType(const std::string &name)
	{
		for (int i = 0; i < Semantics->size(); ++i) {
			if (name == Semantics[i]) {
				return (VertexSemanticType)i;
			}
		}

		NA_ASSERT(false, "Failed to find semantic type with name '%s'", name);
		return VertexSemanticType::UNKNOWN;
	}
}