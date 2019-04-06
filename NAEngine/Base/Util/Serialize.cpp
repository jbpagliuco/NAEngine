#include "Serialize.h"

#include <algorithm>

#include "Color.h"
#include "Debug/Assert.h"
#include "Util/String.h"

#include "pugixml-1.9/pugixml.hpp"

#define RETURN_DEFAULT_IF_INVALID() if (*this == INVALID_DESERIALIZATION_PARAMETER) return def

namespace na
{
	DeserializationParameter INVALID_DESERIALIZATION_PARAMETER = { "" };

	DeserializationParameter& DeserializationParameter::operator[](const char *childName)
	{
		NA_ASSERT(childrenMap.find(childName) != childrenMap.end());

		return childrenMap[childName];
	}

	DeserializationParameter& DeserializationParameter::operator[](int index)
	{
		NA_ASSERT((size_t)index < childrenArray.size());

		return childrenArray[index];
	}

	bool DeserializationParameter::AsBool(bool def)
	{
		RETURN_DEFAULT_IF_INVALID();
		return value == "1" || value == "t" || value == "true" || value == "y" || value == "yes";
	}

	int DeserializationParameter::AsInt(int def)
	{
		RETURN_DEFAULT_IF_INVALID();
		return std::stoi(value);
	}

	float DeserializationParameter::AsFloat(float def)
	{
		RETURN_DEFAULT_IF_INVALID();
		return std::stof(value);
	}

	DirectX::XMFLOAT2 DeserializationParameter::AsFloat2(DirectX::XMFLOAT2 def)
	{
		RETURN_DEFAULT_IF_INVALID();
		
		float x = childrenMap["x"].AsFloat(0.0f);
		float y = childrenMap["y"].AsFloat(0.0f);
		return DirectX::XMFLOAT2(x, y);
	}

	DirectX::XMFLOAT3 DeserializationParameter::AsFloat3(DirectX::XMFLOAT3 def)
	{
		RETURN_DEFAULT_IF_INVALID();

		float x = childrenMap["x"].AsFloat(0.0f);
		float y = childrenMap["y"].AsFloat(0.0f);
		float z = childrenMap["z"].AsFloat(0.0f);
		return DirectX::XMFLOAT3(x, y, z);
	}

	DirectX::XMFLOAT4 DeserializationParameter::AsFloat4(DirectX::XMFLOAT4 def)
	{
		RETURN_DEFAULT_IF_INVALID();

		float x = childrenMap["x"].AsFloat(0.0f);
		float y = childrenMap["y"].AsFloat(0.0f);
		float z = childrenMap["z"].AsFloat(0.0f);
		float w = childrenMap["w"].AsFloat(0.0f);
		return DirectX::XMFLOAT4(x, y, z, w);
	}

	DirectX::XMFLOAT4 DeserializationParameter::AsColor(DirectX::XMFLOAT4 def)
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


	static DeserializationParameter ParseParameterXML(pugi::xml_node &parent)
	{
		DeserializationParameter parentParam;
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
			params[param.name()] = ParseParameterXML(param);
		}

		return params;
	}
}