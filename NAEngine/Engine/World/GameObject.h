#pragma once

#include <vector>

#include "Base/Math/Transform.h"

namespace na
{
	class GameComponent;

	class GameObject
	{
	public:
		GameObject();

		void SetName(const std::string &name);
		std::string GetName()const;

		void AddComponent(GameComponent *component);
		std::vector<GameComponent*>& GetComponents();

	public:
		Transform mTransform;

	private:
		std::string mName;
		std::vector<GameComponent*> mComponents;
	};
}