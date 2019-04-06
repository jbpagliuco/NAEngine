#pragma once

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

	void GameComponentDoFrame();
}