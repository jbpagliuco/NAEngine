#include "Material.h"

namespace na
{
	NA_FACTORY_SETUP(Material);

	bool Material::Initialize(AssetID shaderID, void *propertyData, size_t propertyByteLength)
	{
		mShader = Shader::Get(shaderID);

		mPropertyData = NA_ALLOC_ALIGNED(propertyByteLength, 16);
		memcpy(mPropertyData, propertyData, propertyByteLength);
		mPropertyByteLength = propertyByteLength;

		return true;
	}
	
	void Material::Shutdown()
	{
		NA_SAFE_RELEASE_ASSET_OBJECT(mShader);
		NA_FREE_ALIGNED(mPropertyData);
	}

	void Material::Bind()
	{
		mShader->PSSetBufferData(mPropertyData, mPropertyByteLength);
		mShader->Bind();
	}
}