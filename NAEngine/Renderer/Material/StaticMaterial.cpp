#include "StaticMaterial.h"

namespace na
{
	NA_FACTORY_SETUP(StaticMaterial);

	bool StaticMaterial::Initialize(AssetID shaderID, void *parameterData, size_t parameterByteLength)
	{
		if (!Material::Initialize(shaderID)) {
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

		mConstantBuffer.Shutdown();
	}

	void StaticMaterial::Bind()
	{
		Material::Bind();

		PlatformConstantBuffer *cb = mConstantBuffer.GetBuffer();
		NA_RContext->PSSetConstantBuffers(NA_RStateData->GetUserPSConstantBufferIndex(), 1, &cb);
	}
}