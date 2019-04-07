#include "../Debug/Assert.h"

namespace na
{
	template <typename T>
	Factory<T>::~Factory()
	{
		NA_ASSERT(Instances.size() == 0);
	}

	template <typename T>
	T* Factory<T>::Create(AssetID id)
	{
		if (Exists(id)) {
			return Get(id);
		}

		T *obj = new (NA_ALLOC(sizeof(T))) T();
		obj->mID = id;

		Instances[id].mAsset = obj;

		return obj;
	}

	template <typename T>
	T* Factory<T>::Get(AssetID id)
	{
		return Instances[id].mAsset;
	}

	template <typename T>
	void Factory<T>::Destroy(AssetID id)
	{
		NA_ASSERT_RETURN(Instances.find(id) != Instances.end(), "Asset instance with id %lu doesn't exist!", id);

		Instances[id].mRefCount--;
		if (Instances[id].mRefCount > 0) {
			return;
		}

		T *obj = Instances[id].mAsset;

		obj->Shutdown();
		Instances.erase(id);

		NA_FREE(obj);
	}

	template <typename T>
	void Factory<T>::Destroy(T *obj)
	{
		Destroy(obj->GetID());
	}

	template <typename T>
	bool Factory<T>::Exists(AssetID id)
	{
		return Instances.find(id) != Instances.end();
	}

	template <typename T>
	void Factory<T>::AddRef()
	{
		Instances[mID].mRefCount++;
	}

	template <typename T>
	AssetID Factory<T>::GetID()
	{
		return mID;
	}
}