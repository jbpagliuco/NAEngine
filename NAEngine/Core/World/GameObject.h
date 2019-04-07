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

		void SetName(const char *name);
		const char* GetName()const;

		void AddComponent(GameComponent *component);
		std::vector<GameComponent*>& GetComponents();

	public:
		Transform mTransform;

	private:
		char mName[256];
		std::vector<GameComponent*> mComponents;
	};
}