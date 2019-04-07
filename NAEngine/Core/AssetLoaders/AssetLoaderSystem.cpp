#include "AssetLoaderSystem.h"

#include "Base/Streaming/Stream.h"

#include "MeshLoader.h"

namespace na
{
	bool AssetLoaderSystemInitialize()
	{
		// Register all of our loaders.
		RegisterAssetStreamer("obj", LoadMeshOBJ);

		return true;
	}
}