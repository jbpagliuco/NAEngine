#pragma once

#include <vector>

#include "Base/DataStructures/Singleton.h"

namespace na
{
	class GameObject;

	class World : public Singleton<World>
	{
	public:
		void AddGameObject(GameObject *obj);

	private:
		std::vector<GameObject*> mObjects;
	};
}