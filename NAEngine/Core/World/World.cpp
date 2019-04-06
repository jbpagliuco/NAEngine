#include "World.h"

#include <algorithm>
#include <map>

#include "Base/Debug/Assert.h"
#include "Base/Util/Serialize.h"

#include "Core/World/GameObject.h"
#include "Core/Components/GameComponent.h"

#include "Vendor/pugixml-1.9/pugixml.hpp"

namespace na
{
	NA_CREATE_SINGLETON_INSTANCE(World);

	void World::AddGameObject(GameObject *obj)
	{
		NA_ASSERT_RETURN(std::find(mObjects.begin(), mObjects.end(), obj) == mObjects.end(), "Game object '%s' is already in world.", obj->GetName());

		mObjects.push_back(obj);
	}

	void World::Clear()
	{
		for (auto &obj : mObjects) {
			NA_FREE(obj);
		}

		mObjects.clear();
	}

	void WorldSystemShutdown()
	{
		World::Get()->Clear();
	}

	void LoadWorldFromFile(const char *filename)
	{
		World::Get()->Clear();

		std::vector<GameObject*> gameObjects;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(filename);
		NA_FATAL_ERROR(result, "Failed to load world file '%s'", filename);

		pugi::xml_node root = doc.child("root");

		for (auto &gameObjectXML : root) {
			GameObject *obj = new (NA_ALLOC(sizeof(GameObject))) GameObject();

			char name[256];
			strncpy_s(name, gameObjectXML.child("name").text().as_string(), 256);
			obj->SetName(name);

			// Transform transform = gameObjectXML.child("transform") ? ParseTransformFromXML(gameObjectXML.child("transform")) : Transform();

			// Components
			std::vector<GameComponent*> gameObjectComponents;
			pugi::xml_node componentsArrayXML = gameObjectXML.child("components");
			for (auto &componentXML : componentsArrayXML.children("component")) {
				// Create the component
				const char *componentType = componentXML.attribute("type").value();
				GameComponent *component = CreateComponentFromType(componentType);

				// Deserialize the component
				DeserializationParameterMap params = ParseNode(componentXML);
				component->Deserialize(params);

				// Add the component to this game objects
				obj->AddComponent(component);
				component->SetOwner(obj);
			}

			World::Get()->AddGameObject(obj);
		}
	}
}