#include "CameraComponent.h"

#include "Core/Input/Input.h"
#include "Core/World/GameObject.h"
#include "Renderer/RendererD3D.h"

namespace na
{
	void CameraComponent::Deserialize(DeserializationParameterMap &params)
	{
		mCamera.mFOV = DirectX::XMConvertToRadians(params["fov"].AsFloat());
		mCamera.mNear = params["near"].AsFloat();
		mCamera.mFar = params["far"].AsFloat();

		mSensitivity = params["sensitivity"].AsFloat(3.0f);
	}

	void CameraComponent::Activate()
	{
		mCamera.mTransform = mTransform->GetMatrix();
		NA_Renderer->SetActiveCamera(&mCamera);

		mPitch = mYaw = mRoll = 0.0f;
	}

	void CameraComponent::Deactivate()
	{
		if (NA_Renderer->GetActiveCamera() == &mCamera) {
			NA_Renderer->SetActiveCamera(nullptr);
		}
	}

	void CameraComponent::Update()
	{
		HandleMovementInput();
	}

	void CameraComponent::UpdateLate()
	{
		mCamera.mTransform = mTransform->GetMatrix();
	}

	void CameraComponent::HandleMovementInput()
	{
		mYaw += GetMouseDelta().x * Frametime * mSensitivity;
		mPitch += GetMouseDelta().y * Frametime * mSensitivity;

		DirectX::XMVECTOR newRotation = DirectX::XMQuaternionRotationRollPitchYaw(mPitch, mYaw, mRoll);
		mTransform->SetRotation(newRotation);

		const float baseSpeed = 3.0f;
		const float fastSpeed = 3.0f;
		float fastModifier = (IsShiftDown() || IsKeyDown('F')) ? fastSpeed : 1.0f;

		DirectX::XMFLOAT3 delta;
		delta.x = (IsKeyDown('D') - IsKeyDown('A')) * Frametime * baseSpeed * fastModifier;
		delta.y = (IsKeyDown('E') - IsKeyDown('Q')) * Frametime * baseSpeed * fastModifier;
		delta.z = (IsKeyDown('W') - IsKeyDown('S')) * Frametime * baseSpeed * fastModifier;

		DirectX::XMVECTOR vDelta = DirectX::XMVector3Rotate(DirectX::XMLoadFloat3(&delta), newRotation);
		DirectX::XMStoreFloat3(&delta, vDelta);
		mTransform->Translate(delta);
	}
}