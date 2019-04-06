#include "World.h"

#include <algorithm>

#include "Base/Debug/Assert.h"
#include "Core/Objects/GameObject.h"

namespace na
{
	NA_CREATE_SINGLETON_INSTANCE(World);

	void World::AddGameObject(GameObject *obj)
	{
		NA_ASSERT_RETURN(std::find(mObjects.begin(), mObjects.end(), obj) != mObjects.end(), "Game object '%s' is already in world.", obj->GetName());

		mObjects.push_back(obj);
	}
}