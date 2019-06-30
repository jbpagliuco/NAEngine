#pragma once

#include "RenderableInstance.h"

#include "Renderer/InputLayout.h"
#include "Renderer/Mesh.h"

namespace na
{
	class Material;

	class MeshInstance : public RenderableInstance
	{
	public:
		bool Initialize(AssetID meshID, AssetID matID);
		void Shutdown();

		virtual void Render() override;

	private:
		Mesh *mMesh;
		Material *mMaterial;

		InputLayout mInputLayout;
	};
}