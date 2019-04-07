#pragma once

#include "RenderableInstance.h"

#include "Renderer/Mesh.h"

namespace na
{
	class Material;

	class MeshInstance : public RenderableInstance
	{
	public:
		bool Initialize(const char *meshFilename, Material *material);
		void Shutdown();

		virtual void Render() override;

	private:
		Mesh *mMesh;
		Material *mMaterial;
	};
}