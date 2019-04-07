#include "StaticMeshComponent.h"

#include "Renderer/Scene/Scene.h"

#include "Renderer/Shader.h"
#include "Renderer/Material.h"

#include "Core/AssetLoaders/MeshLoader.h"

namespace na
{
	na::VertexShader vs;
	na::PixelShader ps;
	na::InputLayout il;
	na::Shader shader;

	void StaticMeshComponent::Deserialize(DeserializationParameterMap &params)
	{
		// TODO: Check if the file is already loaded, and re-use if it is.
		const char *filename = params["mesh"].value.c_str();
		Mesh *mesh = LoadMeshFromFile(filename);

		vs.Initialize("data\\red_vs.hlsl");
		ps.Initialize("data\\red_ps.hlsl");

		std::vector<na::InputElement> ilElems;
		na::InputElement ie;
		ie.mSemantic = "POSITION";
		ie.mIndex = 0;
		ie.mFormat = na::Format::R32G32B32_FLOAT;
		ilElems.push_back(ie);
		il.Initialize(ilElems, vs);

		shader.Initialize(&il, &vs, &ps);

		mat.Initialize(&shader);

		mMeshInstance = MeshInstance::Create();
		mMeshInstance->Initialize(mesh, &mat);
	}

	void StaticMeshComponent::Activate()
	{
		Scene::Get()->AddRenderable(mMeshInstance);
	}

	void StaticMeshComponent::Deactivate()
	{
		Scene::Get()->RemoveRenderable(mMeshInstance);

		MeshInstance::Destroy(mMeshInstance);

		vs.Shutdown();
		ps.Shutdown();
		il.Shutdown();
		shader.Shutdown();
		mat.Shutdown();
	}
}