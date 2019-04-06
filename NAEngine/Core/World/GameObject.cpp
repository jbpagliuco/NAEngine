#include "GameObject.h"

#include "Base/Util/Util.h"
#include "Core/Components/GameComponent.h"

namespace na
{
	void GameObject::SetName(const char *name)
	{
		strncpy_s(mName, name, STATIC_ARRAY_SIZE(mName) - 1);
	}

	const char* GameObject::GetName()const
	{
		return mName;
	}

	void GameObject::AddComponent(GameComponent *component)
	{
		mComponents.push_back(component);
	}

	std::vector<GameComponent*>& GameObject::GetComponents()
	{
		return mComponents;
	}
}