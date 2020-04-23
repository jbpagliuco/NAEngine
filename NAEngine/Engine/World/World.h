#pragma once

#include <string>
#include <vector>

#include "Base/DataStructures/Singleton.h"

namespace na
{
	class GameObject;

	class World : public Singleton<World>
	{
	public:
		void AddGameObject(GameObject *obj);

		void Clear();
		
	private:
		std::vector<GameObject*> mObjects;
	};

	void WorldSystemShutdown();

	void LoadWorldFromFile(const std::string &filename);
}