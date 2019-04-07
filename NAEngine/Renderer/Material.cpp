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
		Shader::Destroy(mShader);
	}

	void Material::Bind()
	{
		mShader->Bind();
	}
}