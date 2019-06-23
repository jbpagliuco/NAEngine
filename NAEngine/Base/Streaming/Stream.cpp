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

	AssetID StreamAsset(const std::string &filename, bool async)
	{
		std::string ext = GetFileExt(filename);
		NA_ASSERT_RETURN_VALUE(Streamers.find(ext) != Streamers.end(), INVALID_ASSET_ID, "There is no streamer associated with file extension '%s'.", ext.c_str());

		AssetID id = INVALID_ASSET_ID;
		if (AssetIDs.find(filename) == AssetIDs.end()) {
			id = FirstUnusedAssetID;
			++FirstUnusedAssetID;
		} else {
			id = AssetIDs[filename];
		}

		AssetStreamer streamer = Streamers[ext];
		if (!streamer(id, filename, async)) {
			return INVALID_ASSET_ID;
		}

		AssetIDs[filename] = id;

		return id;
	}

	AssetID GetAssetID(const std::string &filename)
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

	std::string GetAssetFilename(AssetID id)
	{
		for (auto &it : AssetIDs) {
			if (it.second == id) {
				return it.first.c_str();
			}
		}

		return "";
	}

	void RegisterAssetStreamer(const std::string &fileExt, AssetStreamer streamerFunc)
	{
		NA_ASSERT_RETURN(Streamers.find(fileExt) == Streamers.end(), "File ext '%s' has already been registered.", fileExt.c_str());

		Streamers[fileExt] = streamerFunc;
	}
}