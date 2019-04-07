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
	struct DeserializationParameterMap
	{
		std::string value;
		std::map<std::string, std::string> meta;

		std::map<std::string, DeserializationParameterMap> childrenMap;
		std::vector<DeserializationParameterMap> childrenArray;

		DeserializationParameterMap& operator[](const char *childName);
		DeserializationParameterMap& operator[](int index);

		void Insert(const char *name, DeserializationParameterMap value);
		void Insert(int index, DeserializationParameterMap value);

		inline bool operator==(const DeserializationParameterMap &rhs) { return value == rhs.value; }

		const char* AsString(const char *def = "");
		bool AsBool(bool def = false);
		int AsInt(int def = 0);
		
		float AsFloat(float def = 0.0f);
		DirectX::XMFLOAT2 AsFloat2(DirectX::XMFLOAT2 def);
		DirectX::XMFLOAT3 AsFloat3(DirectX::XMFLOAT3 def);
		DirectX::XMFLOAT4 AsFloat4(DirectX::XMFLOAT4 def);

		DirectX::XMFLOAT4 AsColor(DirectX::XMFLOAT4 def);
	};

	extern DeserializationParameterMap INVALID_DESERIALIZATION_PARAMETER;
	
	DeserializationParameterMap ParseNode(pugi::xml_node &parent);
	DeserializationParameterMap ParseFile(const char *filename);
}