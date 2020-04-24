#pragma once

#include "RenderableInstance.h"

#include "Base/Streaming/Stream.h"
#include "Renderer/NGA/NGAInputLayout.h"

namespace na
{
	class MaterialContainer;
	class Mesh;

	class MeshInstance : public RenderableInstance
	{
	public:
		bool Initialize(Mesh *mesh, MaterialContainer *materialContainer);
		void Shutdown();

		virtual void Render(bool bindMaterial = true) override;

	private:
		Mesh *mMesh;
		MaterialContainer *mMaterialContainer;

		NGAInputLayout mInputLayout;
	};
}