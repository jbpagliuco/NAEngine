#include "World.h"

#include <algorithm>
#include <map>

#include "Base/Debug/Assert.h"
#include "Base/Util/Serialize.h"

#include "Engine/World/GameObject.h"
#include "Engine/Components/GameComponent.h"

#include "Vendor/pugixml-1.9/pugixml.hpp"

namespace na
{
	NA_CREATE_SINGLETON_INSTANCE(World);

	void World::AddGameObject(GameObject *obj)
	{
		NA_ASSERT_RETURN(std::find(mObjects.begin(), mObjects.end(), obj) == mObjects.end(), "Game object '%s' is already in world.", obj->GetName().c_str());

		mObjects.push_back(obj);
	}

	void World::Clear()
	{
		for (auto &obj : mObjects) {
			for (auto &component : obj->GetComponents()) {
				component->Deactivate();
			}

			NA_FREE(obj);
		}

		mObjects.clear();
	}

	void WorldSystemShutdown()
	{
		World::Get()->Clear();
	}



	static Transform ParseTransformFromXML(const pugi::xml_node &transformXML)
	{
		auto positionXML = transformXML.child("position");
		DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		if (positionXML) {
			position.x = positionXML.child("x") ? positionXML.child("x").text().as_float() : 0.0f;
			position.y = positionXML.child("y") ? positionXML.child("y").text().as_float() : 0.0f;
			position.z = positionXML.child("z") ? positionXML.child("z").text().as_float() : 0.0f;
		}

		auto rotationXML = transformXML.child("rotation");
		DirectX::XMVECTOR rotation = DirectX::XMQuaternionIdentity();
		if (rotationXML) {
			float x = rotationXML.child("x") ? rotationXML.child("x").text().as_float() : 0.0f;
			float y = rotationXML.child("y") ? rotationXML.child("y").text().as_float() : 0.0f;
			float z = rotationXML.child("z") ? rotationXML.child("z").text().as_float() : 0.0f;

			if (strcmp(rotationXML.attribute("type").as_string(), "lookat") == 0) {
				// Special case. (X, Y, Z) is the point we are looking at.
				static const DirectX::XMVECTOR forwardAxis = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
				const DirectX::XMVECTOR lookAt = DirectX::XMVector3Normalize(DirectX::XMVectorSet(x - position.x, y - position.y, z - position.z, 0.0f));

				const float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(forwardAxis, lookAt));
				if (fabs(dot + 1.0f) < 0.000001f) {
					rotation = DirectX::XMQuaternionRotationNormal(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), DirectX::XM_PI);
				} else if (fabs(dot - 1.0f) < 0.000001f) {
					rotation = DirectX::XMQuaternionIdentity();
				} else {
					const float rotAngle = acos(dot);
					const DirectX::XMVECTOR rotAxis = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(forwardAxis, lookAt));
					rotation = DirectX::XMQuaternionRotationAxis(rotAxis, rotAngle);
				}
			} else {
				rotation = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(x), DirectX::XMConvertToRadians(y), DirectX::XMConvertToRadians(z));
			}
		}

		auto scaleXML = transformXML.child("scale");
		DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
		if (scaleXML) {
			scale.x = scaleXML.child("x") ? scaleXML.child("x").text().as_float() : 1.0f;
			scale.y = scaleXML.child("y") ? scaleXML.child("y").text().as_float() : 1.0f;
			scale.z = scaleXML.child("z") ? scaleXML.child("z").text().as_float() : 1.0f;
		}

		return Transform(position, rotation, scale);
	}

	void LoadWorldFromFile(const std::string &filename)
	{
		World::Get()->Clear();

		std::vector<GameObject*> gameObjects;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(filename.c_str());
		NA_FATAL_ERROR(result, "Failed to load world file '%s'", filename.c_str());

		pugi::xml_node root = doc.child("root");

		for (auto &gameObjectXML : root) {
			GameObject *obj = new (NA_ALLOC(sizeof(GameObject))) GameObject();

			const std::string name = gameObjectXML.child("name").text().as_string();
			obj->SetName(name);

			obj->mTransform = gameObjectXML.child("transform") ? ParseTransformFromXML(gameObjectXML.child("transform")) : Transform();

			// Components
			std::vector<GameComponent*> gameObjectComponents;
			pugi::xml_node componentsArrayXML = gameObjectXML.child("components");
			for (auto &componentXML : componentsArrayXML.children("component")) {
				// Create the component
				const std::string componentType = componentXML.attribute("type").value();
				GameComponent *component = CreateComponentFromType(componentType);

				// Deserialize the component
				DeserializationParameterMap params = ParseNode(componentXML);
				component->Deserialize(params);

				// Add the component to this game objects
				obj->AddComponent(component);
				component->SetOwner(obj);

				// Activate the component
				component->Activate();
			}

			World::Get()->AddGameObject(obj);
		}
	}
}