#include "StaticMeshComponent.h"

#include "Base/Streaming/Stream.h"

#include "Renderer/Material/Material.h"
#include "Renderer/Material/DynamicMaterial.h"
#include "Renderer/Mesh.h"
#include "Renderer/Scene/Scene.h"

#include "Engine/World/GameObject.h"

namespace na
{
	void StaticMeshComponent::Deserialize(DeserializationParameterMap &params)
	{
		mMeshID = RequestAsset(params["mesh"].AsFilepath());

		mMaterialManager.Initialize(params["material"].AsFilepath());
		
		Mesh* mesh = Mesh::Get(mMeshID);
		MaterialContainer* materialContainer = mMaterialManager.GetMaterialContainer();

		mMeshInstance.Initialize(mesh, materialContainer);

		auto &materialParams = params["material"];
		if (materialParams.HasChild("overrides")) {
			materialContainer->CreateDynamicMaterialInstance();

			for (auto &overrideParam : materialParams["overrides"].childrenArray) {
				const std::string type = overrideParam.meta["type"];

				if (type == "texture") {
					mMaterialManager.SetTextureParameter(overrideParam.meta["name"], overrideParam.AsFilepath());
				}
				else if (type == "renderTarget") {
					mMaterialManager.SetRenderTargetParameter(overrideParam.meta["name"], overrideParam.AsFilepath(), overrideParam.meta["map"] == "color");
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
		mMaterialManager.Shutdown();

		ReleaseAsset(mMeshID);
	}

	void StaticMeshComponent::UpdateLate(float deltaTime)
	{
		mMeshInstance.SetWorldTransform(GetOwner()->mTransform.GetMatrix());
	}
}