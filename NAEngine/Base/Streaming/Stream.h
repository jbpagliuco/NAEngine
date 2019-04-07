#pragma once

// --------------------------------------------------
// I have put very little thought into this system.
// --------------------------------------------------

#include <stdint.h>

namespace na
{
	typedef uint64_t AssetID;
	extern AssetID INVALID_ASSET_ID;
	
	AssetID StreamAsset(const char *filename, bool async = false);
	AssetID GetAssetID(const char *filename);
	
	// It is up to each asset system to provide its own storage for
	// every asset.
	typedef bool(*AssetStreamer)(AssetID, const char*, bool);
	void RegisterAssetStreamer(const char *fileExt, AssetStreamer streamerFunc);
}