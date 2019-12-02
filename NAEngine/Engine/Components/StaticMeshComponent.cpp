#include "StaticMeshComponent.h"

#include "Base/Streaming/Stream.h"

#include "Renderer/Scene/Scene.h"
#include "Renderer/Material/Material.h"
#include "Renderer/Material/DynamicMaterial.h"

#include "Engine/World/GameObject.h"

namespace na
{
	void StaticMeshComponent::Deserialize(DeserializationParameterMap &params)
	{
		AssetID meshID = RequestAsset(params["mesh"].AsFilepath());
		AssetID matID = RequestAsset(params["material"].AsFilepath());
		
		mMeshInstance.Initialize(meshID, matID);

		auto &materialParams = params["material"];
		if (materialParams.HasChild("overrides")) {
			DynamicMaterialInstance *dynMat = mMeshInstance.CreateDynamicMaterialInstance();

			for (auto &overrideParam : materialParams["overrides"].childrenArray) {
				const std::string type = overrideParam.meta["type"];

				if (type == "texture") {
					dynMat->SetTextureParameter(overrideParam.meta["name"], overrideParam.AsFilepath());
				} else {
					NA_ASSERT(false, "Type %s not implemented.", type.c_str());
				}
			}
		}
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

	void StaticMeshComponent::UpdateLate(float deltaTime)
	{
		mMeshInstance.SetWorldTransform(GetOwner()->mTransform.GetMatrix());
	}
}