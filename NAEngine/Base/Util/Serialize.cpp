#include "Serialize.h"

#include <algorithm>

#include "Color.h"

namespace na
{
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
				}
				else {
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


	bool DeserializeBool(DeserializationParameterMap &map, const char *name, bool def)
	{
		if (map.find(name) != map.end()) {
			std::string value = map[name].value;
			std::transform(value.begin(), value.end(), value.begin(), ::tolower);

			if (value == "1" || value == "t" || value == "true" || value == "y" || value == "yes") {
				return true;
			}

			return false;
		}

		return def;
	}

	int DeserializeInt(DeserializationParameterMap &map, const char *name, int def)
	{
		if (map.find(name) != map.end()) {
			return std::stoi(map[name].value);
		}

		return def;
	}

	float DeserializeFloat(DeserializationParameterMap &map, const char *name, float def)
	{
		if (map.find(name) != map.end()) {
			return std::stof(map[name].value);
		}

		return def;
	}


	DirectX::XMFLOAT2 DeserializeFloat2(DeserializationParameter &param)
	{
		float x = DeserializeFloat(param.childrenMap, "x", 0.0f);
		float y = DeserializeFloat(param.childrenMap, "y", 0.0f);
		float z = DeserializeFloat(param.childrenMap, "z", 0.0f);
		return DirectX::XMFLOAT2(x, y);
	}

	DirectX::XMFLOAT3 DeserializeFloat3(DeserializationParameter &param)
	{
		float x = DeserializeFloat(param.childrenMap, "x", 0.0f);
		float y = DeserializeFloat(param.childrenMap, "y", 0.0f);
		float z = DeserializeFloat(param.childrenMap, "z", 0.0f);
		return DirectX::XMFLOAT3(x, y, z);
	}

	DirectX::XMFLOAT4 DeserializeFloat4(DeserializationParameter &param)
	{
		float x = DeserializeFloat(param.childrenMap, "x", 0.0f);
		float y = DeserializeFloat(param.childrenMap, "y", 0.0f);
		float z = DeserializeFloat(param.childrenMap, "z", 0.0f);
		float w = DeserializeFloat(param.childrenMap, "w", 0.0f);
		return DirectX::XMFLOAT4(x, y, z, w);
	}

	DirectX::XMFLOAT4 DeserializeColor(DeserializationParameter &param)
	{
		if (param.value.length() != 0) {
			ColorF color(param.value.c_str());
			return DirectX::XMFLOAT4(color.r, color.g, color.b, color.a);
		}

		float r = DeserializeFloat(param.childrenMap, "r", 1.0f);
		float g = DeserializeFloat(param.childrenMap, "g", 0.0f);
		float b = DeserializeFloat(param.childrenMap, "b", 0.0f);
		float a = DeserializeFloat(param.childrenMap, "a", 1.0f);
		return DirectX::XMFLOAT4(r, g, b, a);
	}

	DirectX::XMFLOAT4 DeserializeColor(DeserializationParameterMap &map, const char *name, DirectX::XMFLOAT4 def)
	{
		if (map.find(name) != map.end()) {
			return DeserializeColor(map[name]);
		}

		return def;
	}
}