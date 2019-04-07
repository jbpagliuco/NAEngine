#include "Material.h"

namespace na
{
	NA_FACTORY_SETUP(Material);

	bool Material::Initialize(AssetID shaderID)
	{
		mShader = Shader::Get(shaderID);
		mShader->AddRef();

		return true;
	}
	
	void Material::Shutdown()
	{
		Shader::Destroy(mShader);
	}

	void Material::Bind()
	{
		mShader->Bind();
	}
}