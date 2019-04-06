#include "GameComponent.h"

#include <map>
#include <string>
#include <vector>

#include "Base/Debug/Assert.h"
#include "TestComponent.h"

#define FOREACH_COMPONENT(func, ...) for (auto &iter : ComponentTable) { for (auto &component : iter.second) { component->func(__VA_ARGS__); } }

#define GENERATE_INSTANTIATOR(type) { #type, &InstantiateGameComponent<type> }

namespace na
{
	static std::map<std::string, GameComponent*(*)()> ComponentInstantiators = {
		GENERATE_INSTANTIATOR(TestComponent),
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

	void GameComponent::Deserialize(const DeserializationParameterMap &params)
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
		GameComponent *component = ComponentInstantiators[type]();
		NA_ASSERT_RETURN_VALUE(component != nullptr, nullptr, "Failed to create component of type '%s'", type);

		if (ComponentTable.find(type) == ComponentTable.end()) {
			ComponentTable[type] = ComponentList();
		}

		ComponentTable[type].push_back(component);

		return component;
	}
}