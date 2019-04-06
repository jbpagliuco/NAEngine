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

	private:
		char mName[256];
		std::vector<GameComponent*> mComponents;
	};
}