#include "CameraComponent.h"

#include "Engine/Input/Input.h"
#include "Engine/World/GameObject.h"
#include "Renderer/Renderer.h"

namespace na
{
	void CameraComponent::Deserialize(DeserializationParameterMap &params)
	{
		mCamera.mFOV = ToRadians(params["fov"].AsFloat());
		mCamera.mNear = params["near"].AsFloat();
		mCamera.mFar = params["far"].AsFloat();

		mSensitivity = params["sensitivity"].AsFloat(3.0f);
	}

	void CameraComponent::Activate()
	{
		mCamera.mTransform = *mTransform;
		NA_Renderer->SetActiveCamera(&mCamera);

		mPitch = mYaw = mRoll = 0.0f;
	}

	void CameraComponent::Deactivate()
	{
		if (NA_Renderer->GetActiveCamera() == &mCamera) {
			NA_Renderer->SetActiveCamera(nullptr);
		}
	}

	void CameraComponent::Update(float deltaTime)
	{
		HandleMovementInput(deltaTime);
	}

	void CameraComponent::UpdateLate(float deltaTime)
	{
		mCamera.mTransform = *mTransform;
	}

	void CameraComponent::HandleMovementInput(float deltaTime)
	{
		mYaw += GetMouseDelta().x * deltaTime * mSensitivity;
		mPitch += GetMouseDelta().y * deltaTime * mSensitivity;

		const float baseSpeed = 3.0f;
		const float fastSpeed = 3.0f;
		const float fastModifier = (IsShiftDown() || IsKeyDown('F')) ? fastSpeed : 1.0f;

		Vector3f delta;
		delta.x = (IsKeyDown('D') - IsKeyDown('A')) * deltaTime * baseSpeed * fastModifier;
		delta.y = (IsKeyDown('E') - IsKeyDown('Q')) * deltaTime * baseSpeed * fastModifier;
		delta.z = (IsKeyDown('W') - IsKeyDown('S')) * deltaTime * baseSpeed * fastModifier;

		mTransform->mRotation = Quaternion::FromEuler(mPitch, mYaw, mRoll);
		mTransform->mPosition = Vector(mTransform->mPosition.AsVector3() + (mTransform->mRotation * delta));
	}
}