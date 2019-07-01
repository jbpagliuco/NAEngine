#pragma once

#include "Renderer/Shader/Shader.h"

#include "Renderer/ConstantBuffer.h"

namespace na
{
#define MATERIAL_TYPE_STATIC 0

	class Texture;

	class Material
	{
	public:
		bool Initialize(AssetID shaderID, const std::vector<AssetID> &textures);
		virtual void Shutdown();

		virtual void Bind();

		virtual int GetMaterialType()const = 0;

		inline Shader* GetShader()const { return mShader; }

	protected:
		Shader *mShader;

		ConstantBuffer mConstantBuffer;
		std::vector<Texture*> mTextures;
	};

	Material* GetMaterialByID(AssetID matID);
	void ReleaseMaterial(Material *pMaterial);
}