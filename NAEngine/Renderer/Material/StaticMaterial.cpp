#include "StaticMaterial.h"

namespace na
{
	NA_FACTORY_SETUP(StaticMaterial);

	bool StaticMaterial::Initialize(AssetID shaderID, void *parameterData, size_t parameterByteLength, const std::vector<AssetID> &textures)
	{
		if (!Material::Initialize(shaderID, textures)) {
			return false;
		}

		if (!mConstantBuffer.Initialize(BufferUsage::IMMUTABLE, parameterData, parameterByteLength)) {
			return false;
		}

		return true;
	}
	
	void StaticMaterial::Shutdown()
	{
		Material::Shutdown();
	}
}