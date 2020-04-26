#pragma once

#include <vector>

#include "Base/Math/Transform.h"

#include "Engine/Components/GameComponent.h"

namespace na
{
	class GameObject
	{
	public:
		GameObject();

		void SetName(const std::string &name);
		std::string GetName()const;

		void AddComponent(GameComponent *component);
		std::vector<GameComponent*>& GetComponents();

		template <typename ComponentType>
		ComponentType* GetComponentOfType();

	public:
		Transform mTransform;

	private:
		std::string mName;
		std::vector<GameComponent*> mComponents;
	};
}

namespace na
{
	template <typename ComponentType>
	ComponentType* GameObject::GetComponentOfType()
	{
		// yuck
		ComponentType temp;
		GameComponentType searchType = temp.GetType();

		for (auto& comp : mComponents) {
			if (comp->GetType() == searchType) {
				return (ComponentType*)comp;
			}
		}

		return nullptr;
	}
}