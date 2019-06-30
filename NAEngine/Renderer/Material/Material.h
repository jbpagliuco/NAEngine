#pragma once

#include "Renderer/Shader/Shader.h"

namespace na
{
#define MATERIAL_TYPE_STATIC 0

	class Material
	{
	public:
		bool Initialize(AssetID shaderID);
		virtual void Shutdown();

		virtual void Bind();

		virtual int GetMaterialType()const = 0;

	private:
		Shader *mShader;
	};

	Material* GetMaterialByID(AssetID matID);
	void ReleaseMaterial(Material *pMaterial);
}