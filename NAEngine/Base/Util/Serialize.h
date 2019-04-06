#pragma once

#include <string>
#include <map>
#include <vector>

#include <DirectXMath.h>

namespace pugi {
	class xml_node;
}

namespace na
{
	struct DeserializationParameter
	{
		std::string value;
		std::map<std::string, std::string> meta;

		std::map<std::string, DeserializationParameter> childrenMap;
		std::vector<DeserializationParameter> childrenArray;

		DeserializationParameter& operator[](const char *childName);
		DeserializationParameter& operator[](int index);

		inline bool operator==(const DeserializationParameter &rhs) { return value == rhs.value; }

		bool AsBool(bool def = false);
		int AsInt(int def = 0);
		
		float AsFloat(float def = 0.0f);
		DirectX::XMFLOAT2 AsFloat2(DirectX::XMFLOAT2 def);
		DirectX::XMFLOAT3 AsFloat3(DirectX::XMFLOAT3 def);
		DirectX::XMFLOAT4 AsFloat4(DirectX::XMFLOAT4 def);

		DirectX::XMFLOAT4 AsColor(DirectX::XMFLOAT4 def);
	};

	extern DeserializationParameter INVALID_DESERIALIZATION_PARAMETER;

	typedef std::map<std::string, DeserializationParameter> DeserializationParameterMap;

	DeserializationParameterMap ParseNode(pugi::xml_node &parent);
}