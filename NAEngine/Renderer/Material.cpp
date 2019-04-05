#include "Material.h"

namespace na
{
	bool Material::Initialize(Shader *shader)
	{
		mShader = shader;

		return true;
	}
	
	void Material::Shutdown()
	{
		mShader->Shutdown();
	}

	void Material::Bind()
	{
		mShader->Bind();
	}
}