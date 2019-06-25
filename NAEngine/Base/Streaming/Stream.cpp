#include "Stream.h"

#include <algorithm>
#include <map>

#include "Debug/Assert.h"
#include "File/File.h"

namespace na
{
	AssetID INVALID_ASSET_ID = 0;
	AssetRecord INVALID_ASSET_RECORD;

	static std::map<std::string, AssetType> AssetTypes;

	static std::map<AssetID, AssetRecord> AssetRecords;
	static std::map<std::string, AssetID> AssetIDs;
	static AssetID NextAssetID = 1;


	static AssetID CreateOrGetAssetID(const std::string &filename)
	{
		AssetID id = GetAssetID(filename);
		if (id == INVALID_ASSET_ID) {
			id = NextAssetID;
			++NextAssetID;

			AssetIDs[filename] = id;
		}

		return id;
	}

	static AssetRecord& CreateOrGetAssetRecord(const AssetID &id)
	{
		if (AssetRecords.find(id) != AssetRecords.end()) {
			return AssetRecords[id];
		}

		AssetRecord record;
		record.mID = id;
		
		AssetRecords[id] = record;

		return AssetRecords[id];
	}



	AssetRecord::AssetRecord() :
		mID(INVALID_ASSET_ID),
		mRefCount(0)
	{}

	void AssetRecord::AddRef()
	{
		++mRefCount;
	}

	void AssetRecord::DecRef()
	{
		--mRefCount;
	}



	void RegisterAssetType(const AssetType &type)
	{
#if _NA_DEBUG
		NA_ASSERT(AssetTypes.find(type.mExt) == AssetTypes.end(), false, "Asset type '.%s' is already registered.", type.mExt.c_str());
#endif

		AssetTypes[type.mExt] = type;
	}



	AssetID GetAssetID(const std::string &filename)
	{
		if (AssetIDs.find(filename) != AssetIDs.end()) {
			return AssetIDs[filename];
		}

		return INVALID_ASSET_ID;
	}

	const char* GetAssetFilename(const AssetID &id)
	{
		const auto predicate = [id](std::pair<std::string, AssetID> it)
		{
			return it.second == id;
		};

		const auto &filename = std::find_if(AssetIDs.begin(), AssetIDs.end(), predicate);
		if (filename != AssetIDs.end()) {
			return filename->first.c_str();
		}

		return "";
	}

	AssetRecord& GetAssetRecord(const AssetID &id)
	{
		if (AssetRecords.find(id) != AssetRecords.end()) {
			return AssetRecords[id];
		}

		return INVALID_ASSET_RECORD;
	}



	AssetID RequestAsset(const std::string &filename)
	{
		const std::string ext = GetFileExt(filename);

		NA_ASSERT_RETURN_VALUE(AssetTypes.find(ext) != AssetTypes.end(), INVALID_ASSET_ID, "Requesting asset '%s', but no registered asset type was found!", filename.c_str());
		AssetType &type = AssetTypes[ext];
		
		AssetID id = CreateOrGetAssetID(filename);
		AssetRecord &record = CreateOrGetAssetRecord(id);

		record.AddRef();
		if (record.mRefCount == 1) {
			// The asset hasn't been loaded yet.
			record.mType = &type;
			type.mOnLoad(id, filename);
		}

		return id;
	}

	void ReleaseAsset(const AssetID &id)
	{
		NA_ASSERT_RETURN(id != INVALID_ASSET_ID);

		AssetRecord &record = GetAssetRecord(id);
		NA_ASSERT_RETURN(record != INVALID_ASSET_RECORD);
		
		record.DecRef();
		if (record.mRefCount == 0) {
			record.mType->mOnUnload(id);
			AssetRecords.erase(id);

			AssetIDs.erase(GetAssetFilename(id));
		}
	}

	void ReleaseAsset(const std::string &filename)
	{
		ReleaseAsset(GetAssetID(filename));
	}

	

	bool StreamSystemInit()
	{
		return true;
	}

	void StreamSystemShutdown()
	{
		NA_ASSERT(AssetRecords.size() == 0, "There were left over streaming asset records at shutdown!");
		NA_ASSERT(AssetIDs.size() == 0, "There were left over streaming asset ids at shutdown!");
	}
}