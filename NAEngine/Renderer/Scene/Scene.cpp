#include "Scene.h"

#include <algorithm>

#include "Renderables/RenderableInstance.h"

namespace na
{
	NA_CREATE_SINGLETON_INSTANCE(Scene);

	void Scene::AddRenderable(RenderableInstance *r)
	{
		mRenderables.push_back(r);
	}

	void Scene::RemoveRenderable(RenderableInstance *r)
	{
		mRenderables.erase(std::remove(mRenderables.begin(), mRenderables.end(), r), mRenderables.end());
	}

	const std::vector<RenderableInstance*>& Scene::GetRenderables()const
	{
		return mRenderables;
	}
}