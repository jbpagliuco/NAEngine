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

		// material or materials?
		std::vector<DeserializationParameterMap> materialParams;
		if (params.HasChild("material")) {
			materialParams.push_back(params["material"]);
		}
		else if (params.HasChild("materials")) {
			DeserializationParameterMap &materialArray = params["materials"];
			for (auto& material : materialArray.childrenArray) {
				materialParams.push_back(material);
			}
		}
		else {
			NA_ASSERT(false);
		}

		std::vector<MaterialContainer*> materialContainers;
		for (auto& material : materialParams) {
			const AssetID materialID = RequestAsset(material.AsFilepath());
			mMaterialAssets.push_back(GetMaterialByAssetID(materialID));

			materialContainers.push_back(&mMaterialAssets.back()->GetMaterialContainer());
		}

		Mesh* mesh = Mesh::Get(mMeshID);
		mMeshInstance.Initialize(mesh, materialContainers);

		// Set overrides
		for (int i = 0; i < materialParams.size(); ++i) {
			if (materialParams[i].HasChild("overrides")) {
				// Create the dynamic instance
				materialContainers[i]->CreateDynamicMaterialInstance();

				for (auto& overrideParam : materialParams[i]["overrides"].childrenArray) {
					const std::string type = overrideParam.meta["type"];

					if (type == "texture") {
						mMaterialAssets[i]->SetTextureParameter(overrideParam.meta["name"], overrideParam.AsFilepath());
					}
					else if (type == "renderTarget") {
						mMaterialAssets[i]->SetRenderTargetParameter(overrideParam.meta["name"], overrideParam.AsFilepath(), overrideParam.meta["map"] == "color");
					}
					else {
						NA_ASSERT(false, "Type %s not implemented.", type.c_str());
					}
				}
			}
		}

		// Visibility
		mMeshInstance.mVisible = params["visible"].AsBool(true);
	}

	void StaticMeshComponent::Activate()
	{
		Scene::Get()->AddRenderable(&mMeshInstance);
	}

	void StaticMeshComponent::Deactivate()
	{
		Scene::Get()->RemoveRenderable(&mMeshInstance);

		mMeshInstance.Shutdown();

		for (auto& asset : mMaterialAssets) {
			asset->GetMaterialContainer().Shutdown();
			ReleaseAsset(asset->GetMaterialAssetID());
		}
		ReleaseAsset(mMeshID);
	}

	void StaticMeshComponent::UpdateLate(float deltaTime)
	{
		mMeshInstance.SetWorldTransform(GetOwner()->mTransform.GetMatrix());
	}


	bool StaticMeshComponent::GetVisible()const
	{
		return mMeshInstance.mVisible;
	}

	void StaticMeshComponent::SetVisible(bool visible)
	{
		mMeshInstance.mVisible = visible;
	}
}