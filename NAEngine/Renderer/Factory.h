#pragma once

#include <algorithm>
#include <vector>

#include "Base/Memory/Memory.h"

#define NA_FACTORY_BUILD(T) std::vector<T*> T::Instances = std::vector<T*>()

namespace na
{
	template <typename T>
	class Factory
	{
	public:
		static T* Create()
		{
			T *obj = new (NA_ALLOC(sizeof(T))) T();
			Instances.push_back(obj);

			return obj;
		}

		static void Destroy(T *obj)
		{
			obj->Shutdown();
			Instances.erase(std::remove(Instances.begin(), Instances.end(), obj), Instances.end());
			NA_FREE(obj);
		}

	private:
		static std::vector<T*> Instances;
	};
}