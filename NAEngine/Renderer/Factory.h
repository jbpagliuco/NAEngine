#pragma once

#include <algorithm>
#include <map>

#include "Base/Memory/Memory.h"
#include "Base/Streaming/Stream.h"

#define NA_FACTORY_BUILD(T) std::map<AssetID, T*> T::Instances = std::map<AssetID, T*>()

namespace na
{
	template <typename T>
	class Factory
	{
	public:
		static T* Create(AssetID id)
		{
			if (Exists(id)) {
				return Get(id);
			}

			T *obj = new (NA_ALLOC(sizeof(T))) T();
			obj->mID = id;

			Instances[id] = obj;

			return obj;
		}

		static T* Get(AssetID id)
		{
			return Instances[id];
		}

		static void Destroy(AssetID id)
		{
			T *obj = Instances[id];

			obj->Shutdown();
			Instances.erase(id);

			NA_FREE(obj);
		}

		static void Destroy(T *obj)
		{
			Destroy(obj->GetID());
		}

		static bool Exists(AssetID id)
		{
			return Instances.find(id) != Instances.end();
		}

		AssetID GetID()
		{
			return mID;
		}

	private:
		static std::map<AssetID, T*> Instances;

		AssetID mID;
	};
}