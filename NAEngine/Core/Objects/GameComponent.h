#pragma once

namespace na
{
	class GameObject;

	class GameComponent
	{
	public:
		void SetOwner(GameObject *owner);
		GameObject* GetOwner()const;

		virtual void Deserialize();

		virtual void Activate();
		virtual void Deactivate();
		
		virtual void UpdateEarly(float deltaTime);
		virtual void Update(float deltaTime);
		virtual void UpdateLate(float deltaTime);

	private:
		GameObject *mOwner;
	};
}