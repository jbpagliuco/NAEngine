#pragma once

#include "RenderableInstance.h"

#include "Base/Streaming/Stream.h"
#include "Renderer/Mesh.h"
#include "Renderer/NGA/NGAInputLayout.h"

namespace na
{
	class MaterialContainer;

	class MeshInstance : public RenderableInstance
	{
	public:
		bool Initialize(Mesh *mesh, std::vector<MaterialContainer*> materialContainers);
		void Shutdown();

		virtual void Render(bool bindMaterial = true) override;

	private:
		Mesh *mMesh;
		std::vector<MaterialContainer*> mMaterialContainers;

		NGAInputLayout mInputLayouts[Mesh::MAX_INDEX_BUFFERS];
	};
}