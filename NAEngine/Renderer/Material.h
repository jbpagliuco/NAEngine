#pragma once

#include "Shader.h"

namespace na
{
	class Material : public Factory<Material>
	{
	public:
		bool Initialize(AssetID shaderID);
		void Shutdown();

		void Bind();

	private:
		Shader *mShader;
	};
}