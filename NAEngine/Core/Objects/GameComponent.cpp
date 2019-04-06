#include "GameComponent.h"

namespace na
{

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

	void GameComponent::UpdateEarly(float deltaTime)
	{
	}

	void GameComponent::Update(float deltaTime)
	{
	}

	void GameComponent::UpdateLate(float deltaTime)
	{
	}
}