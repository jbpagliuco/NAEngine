#include "GameComponent.h"

#include <string>
#include <map>

#define FOREACH_COMPONENT(func, ...) for (auto &iter : ComponentTable) { iter.second->func(__VA_ARGS__); }

namespace na
{
	static std::map<std::string, GameComponent*> ComponentTable;

	void GameComponent::SetOwner(GameObject *owner)
	{
		mOwner = owner;
	}

	GameObject* GameComponent::GetOwner()const
	{
		return mOwner;
	}

	void GameComponent::Deserialize()
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
}