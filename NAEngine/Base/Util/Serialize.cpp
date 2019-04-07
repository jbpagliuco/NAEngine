#include "Serialize.h"

#include <algorithm>

#include "Color.h"
#include "Debug/Assert.h"
#include "Util/String.h"

#include "pugixml-1.9/pugixml.hpp"

#define RETURN_DEFAULT_IF_INVALID() if (*this == INVALID_DESERIALIZATION_PARAMETER) return def

namespace na
{
	// Lol what is this
	DeserializationParameterMap INVALID_DESERIALIZATION_PARAMETER = { "~*INVALID*~" };

	DeserializationParameterMap& DeserializationParameterMap::operator[](const char *childName)
	{
		if (childrenMap.find(childName) == childrenMap.end()) {
			return INVALID_DESERIALIZATION_PARAMETER;
		}

		return childrenMap[childName];
	}

	DeserializationParameterMap& DeserializationParameterMap::operator[](int index)
	{
		NA_ASSERT_RETURN_VALUE((size_t)index < childrenArray.size(), INVALID_DESERIALIZATION_PARAMETER, "Failed to find value at index %d", index);

		return childrenArray[index];
	}

	void DeserializationParameterMap::Insert(const char *name, DeserializationParameterMap map)
	{
		childrenMap[name] = map;
	}

	void DeserializationParameterMap::Insert(int index, DeserializationParameterMap map)
	{
		childrenArray.insert(childrenArray.begin() + index, map);
	}

	const char* DeserializationParameterMap::AsString(const char *def)
	{
		return value.c_str();
	}

	bool DeserializationParameterMap::AsBool(bool def)
	{
		RETURN_DEFAULT_IF_INVALID();
		return value == "1" || value == "t" || value == "true" || value == "y" || value == "yes";
	}

	int DeserializationParameterMap::AsInt(int def)
	{
		RETURN_DEFAULT_IF_INVALID();
		return std::stoi(value);
	}

	float DeserializationParameterMap::AsFloat(float def)
	{
		RETURN_DEFAULT_IF_INVALID();
		return std::stof(value);
	}

	DirectX::XMFLOAT2 DeserializationParameterMap::AsFloat2(DirectX::XMFLOAT2 def)
	{
		RETURN_DEFAULT_IF_INVALID();
		
		float x = childrenMap["x"].AsFloat(0.0f);
		float y = childrenMap["y"].AsFloat(0.0f);
		return DirectX::XMFLOAT2(x, y);
	}

	DirectX::XMFLOAT3 DeserializationParameterMap::AsFloat3(DirectX::XMFLOAT3 def)
	{
		RETURN_DEFAULT_IF_INVALID();

		float x = childrenMap["x"].AsFloat(0.0f);
		float y = childrenMap["y"].AsFloat(0.0f);
		float z = childrenMap["z"].AsFloat(0.0f);
		return DirectX::XMFLOAT3(x, y, z);
	}

	DirectX::XMFLOAT4 DeserializationParameterMap::AsFloat4(DirectX::XMFLOAT4 def)
	{
		RETURN_DEFAULT_IF_INVALID();

		float x = childrenMap["x"].AsFloat(0.0f);
		float y = childrenMap["y"].AsFloat(0.0f);
		float z = childrenMap["z"].AsFloat(0.0f);
		float w = childrenMap["w"].AsFloat(0.0f);
		return DirectX::XMFLOAT4(x, y, z, w);
	}

	DirectX::XMFLOAT4 DeserializationParameterMap::AsColor(DirectX::XMFLOAT4 def)
	{
		RETURN_DEFAULT_IF_INVALID();

		// String
		if (value.length() != 0) {
			ColorF color(value.c_str());
			return DirectX::XMFLOAT4(color.r, color.g, color.b, color.a);
		}

		float r = childrenMap["r"].AsFloat(0.0f);
		float g = childrenMap["g"].AsFloat(0.0f);
		float b = childrenMap["b"].AsFloat(0.0f);
		float a = childrenMap["a"].AsFloat(0.0f);
		return DirectX::XMFLOAT4(r, g, b, a);
	}


	static DeserializationParameterMap ParseParameterXML(pugi::xml_node &parent)
	{
		DeserializationParameterMap parentParam;
		parentParam.value = parent.text().as_string();

		// Is array?
		bool isArray = false;
		const char *lastName = nullptr;
		for (auto &child : parent.children()) {
			if (lastName) {
				if (strcmp(lastName, child.name()) == 0) {
					isArray = true;
					break;
				}
			}
			lastName = child.name();
		}

		// Gather attributes (meta)
		for (auto &attr : parent.attributes()) {
			parentParam.meta[attr.name()] = attr.value();
		}

		// Gather properties
		for (auto &child : parent.children()) {
			if (child.type() == pugi::xml_node_type::node_element) {
				if (isArray) {
					parentParam.childrenArray.push_back(ParseParameterXML(child));
				} else {
					parentParam.childrenMap[child.name()] = ParseParameterXML(child);
				}
			}
		}

		if (parentParam.childrenMap.size() == 1) {
			parentParam.childrenArray.push_back(parentParam.childrenMap.begin()->second);
		}

		return parentParam;
	}

	DeserializationParameterMap ParseNode(pugi::xml_node &parent)
	{
		DeserializationParameterMap params;
		for (auto &param : parent.children()) {
			params.Insert(param.name(), ParseParameterXML(param));
		}

		return params;
	}

	DeserializationParameterMap ParseFile(const char *filename)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(filename);
		NA_ASSERT_RETURN_VALUE(result, INVALID_DESERIALIZATION_PARAMETER, "Failed to load XML file '%s'", filename);

		pugi::xml_node root = doc.child("root");
		return ParseNode(root);
	}
}