#pragma once

#include "Renderer/Shader/Shader.h"

namespace na
{
#define MATERIAL_TYPE_STATIC 0
#define MATERIAL_TYPE_DYNAMIC 1
	
	class Material
	{
	public:
		bool Initialize(Shader *shader);
		virtual void Shutdown();

		virtual void Bind();

		virtual AssetID GetID()const = 0;
		virtual int GetMaterialType()const = 0;

		inline Shader* GetShader()const { return mShader; }

	private:
		Shader *mShader;
	};

	Material* GetMaterialByID(AssetID matID);
	void ReleaseMaterial(Material *pMaterial);
}