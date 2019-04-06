#pragma once

#include "Base/Memory/Memory.h"

namespace na
{
	class GameObject;

	extern float Frametime;

	class GameComponent
	{
	public:
		void SetOwner(GameObject *owner);
		GameObject* GetOwner()const;

		virtual void Deserialize();

		virtual void Activate();
		virtual void Deactivate();
		
		virtual void UpdateEarly();
		virtual void Update();
		virtual void UpdateLate();

	private:
		GameObject *mOwner;
	};

	// System Do Frame
	void GameComponentDoFrame();

	GameComponent* CreateComponentFromType(const char *type);

	template <typename T>
	GameComponent* InstantiateGameComponent()
	{
		void *mem = NA_ALLOC(sizeof(T));
		return new (mem) T();
	}
}