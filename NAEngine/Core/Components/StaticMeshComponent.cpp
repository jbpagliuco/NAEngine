#include "StaticMeshComponent.h"

#include "Base/Streaming/Stream.h"

#include "Renderer/Scene/Scene.h"

#include "Renderer/Shader.h"
#include "Renderer/Material.h"


namespace na
{
	void StaticMeshComponent::Deserialize(DeserializationParameterMap &params)
	{
		AssetID shaderID = StreamAsset(params["shader"].AsString());
		Shader *shader = Shader::Get(shaderID);
		shader->AddRef();
		mat.Initialize(shader);

		const char *meshFilename = params["mesh"].AsString();
		mMeshInstance.Initialize(meshFilename, &mat);
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
}