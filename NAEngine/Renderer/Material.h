#pragma once

#include "Shader.h"

namespace na
{
	class Material
	{
	public:
		bool Initialize(Shader *shader);
		void Shutdown();

		void Bind();

	private:
		Shader *mShader;
	};
}