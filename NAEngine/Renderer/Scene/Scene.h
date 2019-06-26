#pragma once

#include <vector>

#include "Base/DataStructures/Singleton.h"

namespace na
{
	struct Light;
	class RenderableInstance;

	class Scene : public Singleton<Scene>
	{
	public:
		void AddRenderable(RenderableInstance *r);
		void RemoveRenderable(RenderableInstance *r);

		void AddLight(Light *light);
		void RemoveLight(Light *light);

		const std::vector<RenderableInstance*>& GetRenderables()const;
		const std::vector<Light*>& GetLights()const;

	private:
		std::vector<RenderableInstance*> mRenderables;
		std::vector<Light*> mLights;
	};
}