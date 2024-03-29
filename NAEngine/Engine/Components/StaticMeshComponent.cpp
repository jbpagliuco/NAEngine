#include "StaticMeshComponent.h"

#include "Base/Streaming/Stream.h"

#include "Renderer/Material/Material.h"
#include "Renderer/Material/DynamicMaterial.h"
#include "Renderer/Mesh.h"
#include "Renderer/Scene/Scene.h"

#include "Engine/World/GameObject.h"
#include "Engine/Assets/MaterialAsset.h"

namespace na
{
	void StaticMeshComponent::Deserialize(DeserializationParameterMap &params)
	{
		mMeshID = RequestAsset(params["mesh"].AsFilepath());

		AssetID materialID = RequestAsset(params["material"].AsFilepath());
		mMaterialAsset = GetMaterialByAssetID(materialID);
		
		Mesh* mesh = Mesh::Get(mMeshID);
		MaterialContainer& materialContainer = mMaterialAsset->GetMaterialContainer();

		mMeshInstance.Initialize(mesh, &materialContainer);

		// Set overrides
		auto &materialParams = params["material"];
		if (materialParams.HasChild("overrides")) {
			// Create the dynamic instance
			materialContainer.CreateDynamicMaterialInstance();

			for (auto &overrideParam : materialParams["overrides"].childrenArray) {
				const std::string type = overrideParam.meta["type"];

				if (type == "texture") {
					mMaterialAsset->SetTextureParameter(overrideParam.meta["name"], overrideParam.AsFilepath());
				}
				else if (type == "renderTarget") {
					mMaterialAsset->SetRenderTargetParameter(overrideParam.meta["name"], overrideParam.AsFilepath(), overrideParam.meta["map"] == "color");
				}
				else {
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

		ReleaseAsset(mMaterialAsset->GetMaterialAssetID());
		ReleaseAsset(mMeshID);
	}

	void StaticMeshComponent::UpdateLate(float deltaTime)
	{
		mMeshInstance.SetWorldTransform(GetOwner()->mTransform.GetMatrix());
	}
}