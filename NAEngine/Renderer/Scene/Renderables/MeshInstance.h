#pragma once

#include "RenderableInstance.h"

#include "Base/Streaming/Stream.h"
#include "Renderer/NGA/NGAInputLayout.h"

namespace na
{
	class Material;
	class Mesh;
	class DynamicMaterialInstance;

	class MeshInstance : public RenderableInstance
	{
	public:
		bool Initialize(AssetID meshID, AssetID matID);
		void Shutdown();

		DynamicMaterialInstance* CreateDynamicMaterialInstance();
		DynamicMaterialInstance* GetDynamicMaterialInstance();

		virtual void Render() override;

	private:
		Mesh *mMesh;
		Material *mMaterial;

		DynamicMaterialInstance *mDynMaterialInst;

		NGAInputLayout mInputLayout;
	};
}