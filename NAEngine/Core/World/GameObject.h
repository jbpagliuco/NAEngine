#pragma once

#include <vector>

namespace na
{
	class GameComponent;

	class GameObject
	{
	public:
		void SetName(const char *name);
		const char* GetName()const;

		void AddComponent(GameComponent *component);
		std::vector<GameComponent*>& GetComponents();

	private:
		char mName[256];
		std::vector<GameComponent*> mComponents;
	};
}