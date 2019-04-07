#include "GameComponent.h"

#include <map>
#include <string>
#include <vector>

#include "Base/Debug/Assert.h"

#include "CameraComponent.h"
#include "StaticMeshComponent.h"

#define FOREACH_COMPONENT(func, ...) for (auto &iter : ComponentTable) { for (auto &component : iter.second) { component->func(__VA_ARGS__); } }

#define GENERATE_INSTANTIATOR(type) { #type, &InstantiateGameComponent<type> }

namespace na
{
	static std::map<std::string, GameComponent*(*)()> ComponentInstantiators = {
		GENERATE_INSTANTIATOR(CameraComponent),
		GENERATE_INSTANTIATOR(StaticMeshComponent)
	};

	typedef std::vector<GameComponent*> ComponentList;
	static std::map<std::string, ComponentList> ComponentTable;

	void GameComponent::SetOwner(GameObject *owner)
	{
		mOwner = owner;
	}

	GameObject* GameComponent::GetOwner()const
	{
		return mOwner;
	}

	void GameComponent::Deserialize(DeserializationParameterMap &params)
	{
	}

	void GameComponent::Activate()
	{
	}

	void GameComponent::Deactivate()
	{
	}

	void GameComponent::UpdateEarly()
	{
	}

	void GameComponent::Update()
	{
	}

	void GameComponent::UpdateLate()
	{
	}


	void GameComponentDoFrame()
	{
		FOREACH_COMPONENT(UpdateEarly);
		FOREACH_COMPONENT(Update);
		FOREACH_COMPONENT(UpdateLate);
	}

	void GameComponentShutdown()
	{
		for (auto &iter : ComponentTable) {
			ComponentList components = iter.second;
			for (auto &component : components) {
				NA_FREE(component);
			}
		}
	}

	GameComponent* CreateComponentFromType(const char *type)
	{
		NA_ASSERT_RETURN_VALUE(ComponentInstantiators.find(type) != ComponentInstantiators.end(), nullptr, "Failed to find instantiator for component type '%s'. Did you forget to add it to this file?", type);
		GameComponent *component = ComponentInstantiators[type]();
		NA_ASSERT_RETURN_VALUE(component != nullptr, nullptr, "Failed to create component of type '%s'", type);

		if (ComponentTable.find(type) == ComponentTable.end()) {
			ComponentTable[type] = ComponentList();
		}

		ComponentTable[type].push_back(component);

		return component;
	}
}