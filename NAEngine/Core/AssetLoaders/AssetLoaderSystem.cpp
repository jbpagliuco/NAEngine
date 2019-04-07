#include "AssetLoaderSystem.h"

#include "Base/Streaming/Stream.h"

#include "MaterialLoader.h"
#include "MeshLoader.h"

namespace na
{
	bool AssetLoaderSystemInitialize()
	{
		// Register all of our loaders.
		RegisterAssetStreamer("obj", LoadMeshOBJ);
		RegisterAssetStreamer("shaderx", LoadShaderx);
		RegisterAssetStreamer("matx", LoadMatx);

		return true;
	}
}