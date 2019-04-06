#pragma once

#include "RenderableInstance.h"

#include "Renderer/Mesh.h"

namespace na
{
	class Material;

	class MeshInstance : public RenderableInstance, public Factory<MeshInstance>
	{
	public:
		bool Initialize(const MeshData &meshData, Material *material);
		void Shutdown();

		virtual void Render() override;

	private:
		Mesh *mMesh;
		Material *mMaterial;
	};
}