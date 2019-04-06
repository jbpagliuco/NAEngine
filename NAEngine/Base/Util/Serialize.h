#pragma once

#include <string>
#include <map>
#include <vector>

#include <DirectXMath.h>

#include <pugixml-1.9/pugixml.hpp>

#include "Debug/Assert.h"

namespace na
{
	struct DeserializationParameter
	{
		std::map<std::string, std::string> meta;
		std::string value;

		std::map<std::string, DeserializationParameter> childrenMap;
		std::vector<DeserializationParameter> childrenArray;

		DeserializationParameter& operator[](const char *childName)
		{
			return childrenMap[childName];
		}

		DeserializationParameter& operator[](int index)
		{
			NA_ASSERT((size_t)index < childrenArray.size());

			return childrenArray[index];
		}
	};

	typedef std::map<std::string, DeserializationParameter> DeserializationParameterMap;

	DeserializationParameterMap ParseNode(pugi::xml_node &parent);

	bool DeserializeBool(DeserializationParameterMap &map, const char *name, bool def);
	int DeserializeInt(DeserializationParameterMap &map, const char *name, int def);
	float DeserializeFloat(DeserializationParameterMap &map, const char *name, float def);

	template <size_t N>
	void DeserializeStringLower(char(&buf)[N], DeserializationParameterMap &map, const char *name, const char *def);

	DirectX::XMFLOAT2 DeserializeFloat2(DeserializationParameter &param);
	DirectX::XMFLOAT3 DeserializeFloat3(DeserializationParameter &param);
	DirectX::XMFLOAT4 DeserializeFloat4(DeserializationParameter &param);

	DirectX::XMFLOAT4 DeserializeColor(DeserializationParameter &param);
	DirectX::XMFLOAT4 DeserializeColor(DeserializationParameterMap &map, const char *name, DirectX::XMFLOAT4 def);





	template <size_t N>
	void DeserializeStringLower(char(&buf)[N], DeserializationParameterMap &map, const char *name, const char *def)
	{
		if (map.find(name) != map.end()) {
			strncpy_s(buf, map[name].value.c_str(), N);
			fes::ToLowerCase(buf);
		}
		else {
			strncpy_s(buf, def, N);
		}
	}
}