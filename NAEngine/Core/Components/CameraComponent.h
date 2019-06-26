#pragma once

#include "GameComponent.h"

#include "Renderer/Scene/Camera.h"

namespace na
{
	class CameraComponent : public GameComponent
	{
	public:
		virtual void Deserialize(DeserializationParameterMap &params) override;

		virtual void Activate() override;
		virtual void Deactivate() override;

		virtual void Update() override;
		virtual void UpdateLate() override;

	private:
		void HandleMovementInput();

	private:
		Camera mCamera;

		float mPitch;
		float mYaw;
		float mRoll;

		float mSensitivity;
	};
}