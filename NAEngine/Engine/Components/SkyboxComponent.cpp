#include "SkyboxComponent.h"

#include "Renderer/Resources/Skybox.h"

#include "Engine/Components/CameraComponent.h"
#include "Engine/World/GameObject.h"

namespace na
{
	void SkyboxComponent::Deserialize(DeserializationParameterMap& params)
	{
		mSkyboxID = RequestAsset(params["skybox"].AsFilepath());
	}

	void SkyboxComponent::Activate()
	{
		CameraComponent* camera = GetOwner()->GetComponentOfType<CameraComponent>();
		NA_ASSERT(camera != nullptr, "Skybox component is on object with no camera component.");
	}

	void SkyboxComponent::Deactivate()
	{
		ReleaseAsset(mSkyboxID);
	}

	Skybox* SkyboxComponent::GetSkybox()
	{
		return Skybox::Get(mSkyboxID);
	}
}