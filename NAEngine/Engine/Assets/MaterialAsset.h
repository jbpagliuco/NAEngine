#pragma once

#include "Base/Streaming/Stream.h"

namespace na
{
	class Material;

	bool MaterialSystemInit();
	void MaterialSystemShutdown();

	Material* GetMaterialByAssetID(AssetID id);
}