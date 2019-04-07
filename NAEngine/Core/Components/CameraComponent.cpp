#include "CameraComponent.h"

#include "Core/World/GameObject.h"

#include "Renderer/RendererD3D.h"

namespace na
{
	void CameraComponent::Deserialize(DeserializationParameterMap &params)
	{
		mCamera.mFOV = DirectX::XMConvertToRadians(params["fov"].AsFloat());
		mCamera.mNear = params["near"].AsFloat();
		mCamera.mFar = params["far"].AsFloat();
	}

	void CameraComponent::Activate()
	{
		mCamera.mTransform = GetOwner()->mTransform.GetMatrix();
		NA_Renderer->SetActiveCamera(&mCamera);
	}

	void CameraComponent::Deactivate()
	{
		if (NA_Renderer->GetActiveCamera() == &mCamera) {
			NA_Renderer->SetActiveCamera(nullptr);
		}
	}

	void CameraComponent::Update()
	{
		static float t = 0.0f;
		t += Frametime * 1.0f;

		float radius = 2.0f;
		float height = 2.0f;
		DirectX::XMFLOAT3 pos(radius * cosf(t), height, radius * sinf(t));
		GetOwner()->mTransform.SetPosition(pos);
		GetOwner()->mTransform.SetLookAt(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	}

	void CameraComponent::UpdateLate()
	{
		mCamera.mTransform = GetOwner()->mTransform.GetMatrix();
	}
}