#include "StaticMeshComponent.h"

#include "Base/Streaming/Stream.h"

#include "Renderer/Scene/Scene.h"

#include "Renderer/Shader.h"
#include "Renderer/Material.h"

#include "Core/World/GameObject.h"

namespace na
{
	void StaticMeshComponent::Deserialize(DeserializationParameterMap &params)
	{
		AssetID meshID = StreamAsset(params["mesh"].AsString());
		AssetID matID = StreamAsset(params["material"].AsString());

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