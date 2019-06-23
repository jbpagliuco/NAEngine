#pragma once

#include <stdint.h>

#include <algorithm>
#include <map>

#include "../Memory/Memory.h"

#define NA_FACTORY_SETUP(T) std::map<AssetID, Factory<T>::Asset> T::Instances = std::map<AssetID, Factory<T>::Asset>()

namespace na
{
	typedef uint64_t AssetID;
	extern AssetID INVALID_ASSET_ID;
	
	AssetID StreamAsset(const std::string &filename, bool async = false);
	AssetID GetAssetID(const std::string &filename);
	std::string GetAssetFilename(AssetID id);
	
	// It is up to each asset system to provide its own storage for
	// every asset.
	typedef bool(*AssetStreamer)(AssetID, const std::string&, bool);
	void RegisterAssetStreamer(const std::string &fileExt, AssetStreamer streamerFunc);
	

	template <typename T>
	class Factory
	{
	public:
		virtual ~Factory();

		static T* Create(AssetID id);

		static void Destroy(AssetID id);
		static void Destroy(T *obj);

		static T* Get(AssetID id);
		static bool Exists(AssetID id);

		void AddRef();

		AssetID GetID();

		struct Asset
		{
			T* mAsset = nullptr;
			int mRefCount = 0;
		};

	private:
		static std::map<AssetID, Asset> Instances;

		AssetID mID;
	};
}

#include "Stream.inl"