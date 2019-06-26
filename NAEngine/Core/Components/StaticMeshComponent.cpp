#include "StaticMeshComponent.h"

#include "Base/Streaming/Stream.h"

#include "Renderer/Scene/Scene.h"
#include "Renderer/Material.h"

#include "Core/World/GameObject.h"

namespace na
{
	void StaticMeshComponent::Deserialize(DeserializationParameterMap &params)
	{
		AssetID meshID = RequestAsset(params["mesh"].AsFilepath());
		AssetID matID = RequestAsset(params["material"].AsFilepath());

		mMeshInstance.Initialize(meshID, matID);
	}

	void StaticMeshComponent::Activate()
	{
		Scene::Get()->AddRenderable(&mMeshInstance);
	}

	void StaticMeshComponent::Deactivate()
	{
		Scene::Get()->RemoveRenderable(&mMeshInstance);

		mMeshInstance.Shutdown();
	}

	void StaticMeshComponent::UpdateLate()
	{
		mMeshInstance.SetWorldTransform(GetOwner()->mTransform.GetMatrix());
	}
}