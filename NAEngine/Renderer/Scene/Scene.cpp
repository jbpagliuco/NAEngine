#include "Scene.h"

#include "Renderables/RenderableInstance.h"

namespace na
{
	Scene* Scene::GetInstance()
	{
		static Scene scene;

		return &scene;
	}

	void Scene::AddRenderable(RenderableInstance *r)
	{
		mRenderables.push_back(r);
	}

	const std::vector<RenderableInstance*>& Scene::GetRenderables()const
	{
		return mRenderables;
	}
}