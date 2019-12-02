#pragma once

#include "Base/Memory/Memory.h"
#include "Base/Util/Serialize.h"

namespace na
{
	class GameObject;
	class Transform;

	extern float Frametime;

	class GameComponent
	{
	public:
		void SetOwner(GameObject *owner);
		GameObject* GetOwner()const;

		virtual void Deserialize(DeserializationParameterMap &params);

		virtual void Activate();
		virtual void Deactivate();
		
		virtual void UpdateEarly(float deltaTime);
		virtual void Update(float deltaTime);
		virtual void UpdateLate(float deltaTime);

	protected:
		GameObject *mOwner;
		Transform *mTransform;
	};

	// System Callbacks
	void GameComponentDoFrame();
	void GameComponentShutdown();

	GameComponent* CreateComponentFromType(const std::string &type);
}