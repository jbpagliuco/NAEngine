#include "Material.h"

namespace na
{
	NA_FACTORY_SETUP(Material);

	bool Material::Initialize(AssetID shaderID, void *propertyData, size_t propertyByteLength)
	{
		mShader = Shader::Get(shaderID);
		mShader->AddRef();

		mPropertyData = NA_ALLOC_ALIGNED(propertyByteLength, 16);
		memcpy(mPropertyData, propertyData, propertyByteLength);
		mPropertyByteLength = propertyByteLength;

		return true;
	}
	
	void Material::Shutdown()
	{
		Shader::Destroy(mShader);

		NA_FREE_ALIGNED(mPropertyData);
	}

	void Material::Bind()
	{
		mShader->PSSetBufferData(mPropertyData, mPropertyByteLength);
		mShader->Bind();
	}
}