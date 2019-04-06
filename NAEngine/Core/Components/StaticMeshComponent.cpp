#include "StaticMeshComponent.h"

#include "Renderer/Scene/Scene.h"

#include "Renderer/Shader.h"
#include "Renderer/Material.h"

namespace na
{
	na::VertexShader vs;
	na::PixelShader ps;
	na::InputLayout il;
	na::Shader shader;

	void StaticMeshComponent::Deserialize(DeserializationParameterMap &params)
	{
		MeshData mesh;
	
		float verts[] = { 0.0f, 0.5f, 0.0f,     0.5f, 0.0f, 0.0f,      -0.5f, 0.0f, 0.0f, };
		mesh.vertices = verts;
		mesh.numVertices = 3;
		mesh.vertexStride = sizeof(float) * 3;

		IndexType inds[] = { 0, 1, 2 };
		mesh.indices = inds;
		mesh.numIndices = 3;

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