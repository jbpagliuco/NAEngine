#include "Stream.h"

#include <map>
#include <string>

#include "Debug/Assert.h"
#include "File/File.h"

namespace na
{
	AssetID INVALID_ASSET_ID = 0;

	static std::map<std::string, AssetStreamer> Streamers;

	static AssetID FirstUnusedAssetID = 1; // OMEGALUL
	static std::map<std::string, AssetID> AssetIDs; // Filename -> AssetID. This is terrible.

	AssetID StreamAsset(const char *filename, bool async)
	{
		const char *ext = GetFileExt(filename);
		NA_ASSERT_RETURN_VALUE(Streamers.find(ext) != Streamers.end(), INVALID_ASSET_ID, "There is no streamer associated with file extension '%s'.", ext);

		AssetID id = FirstUnusedAssetID;

		AssetStreamer streamer = Streamers[ext];
		if (!streamer(id, filename, async)) {
			return INVALID_ASSET_ID;
		}

		++FirstUnusedAssetID;
		return id;
	}

	AssetID GetAssetID(const char *filename)
	{
		// Doesn't exist yet? Create it.
		if (AssetIDs.find(filename) == AssetIDs.end()) {
			AssetID id = FirstUnusedAssetID;
			AssetIDs[filename] = id;
			++FirstUnusedAssetID;
			return id;
		}

		return AssetIDs[filename];
	}

	void RegisterAssetStreamer(const char *fileExt, AssetStreamer streamerFunc)
	{
		NA_ASSERT_RETURN(Streamers.find(fileExt) == Streamers.end(), "File ext '%s' has already been registered.", fileExt);

		Streamers[fileExt] = streamerFunc;
	}
}