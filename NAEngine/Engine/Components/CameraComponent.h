#pragma once

#include "GameComponent.h"

#include "Renderer/Resources/Texture.h"
#include "Renderer/Scene/Camera.h"

namespace na
{
	class CameraComponent : public GameComponent
	{
	public:
		virtual void Deserialize(DeserializationParameterMap &params) override;

		virtual void Activate() override;
		virtual void Deactivate() override;

		virtual void Update(float deltaTime) override;
		virtual void UpdateLate(float deltaTime) override;

	private:
		void HandleMovementInput(float deltaTime);

	private:
		Camera mCamera;
		Texture mRenderTarget;

		float mPitch;
		float mYaw;
		float mRoll;

		float mSensitivity;
	};
}