#include "StaticMeshComponent.h"

#include "Base/Streaming/Stream.h"

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
		vs.Initialize("data\\red_vs.hlsl");
		ps.Initialize("data\\red_ps.hlsl");

		std::vector<na::InputElement> ilElems;

		na::InputElement ie;
		ie.mSemantic = "POSITION";
		ie.mIndex = 0;
		ie.mFormat = Format::R32G32B32_FLOAT;
		ilElems.push_back(ie);

		ie.mSemantic = "NORMAL";
		ie.mIndex = 0;
		ie.mFormat = Format::R32G32B32_FLOAT;
		ilElems.push_back(ie);

		ie.mSemantic = "TEXCOORD";
		ie.mIndex = 0;
		ie.mFormat = Format::R32G32_FLOAT;
		ilElems.push_back(ie);

		il.Initialize(ilElems, vs);

		shader.Initialize(&il, &vs, &ps);

		mat.Initialize(&shader);

		const char *meshFilename = params["mesh"].value.c_str();
		mMeshInstance.Initialize(meshFilename, &mat);
	}

	void StaticMeshComponent::Activate()
	{
		Scene::Get()->AddRenderable(&mMeshInstance);
	}

	void StaticMeshComponent::Deactivate()
	{
		mMeshInstance.Shutdown();

		Scene::Get()->RemoveRenderable(&mMeshInstance);

		vs.Shutdown();
		ps.Shutdown();
		il.Shutdown();
		shader.Shutdown();
		mat.Shutdown();
	}
}