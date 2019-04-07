#pragma once

#include "Shader.h"

namespace na
{
	class Material : public Factory<Material>
	{
	public:
		bool Initialize(AssetID shaderID, void *properties, size_t propertiesSize);
		void Shutdown();

		void Bind();

	private:
		Shader *mShader;

		void *mPropertyData;
		size_t mPropertyByteLength;
	};
}