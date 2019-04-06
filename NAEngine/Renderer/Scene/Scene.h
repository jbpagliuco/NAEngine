#pragma once

#include <vector>

#include "Base/DataStructures/Singleton.h"

namespace na
{
	class RenderableInstance;

	class Scene : public Singleton<Scene>
	{
	public:
		void AddRenderable(RenderableInstance *r);
		void RemoveRenderable(RenderableInstance *r);

		const std::vector<RenderableInstance*>& GetRenderables()const;

	private:
		std::vector<RenderableInstance*> mRenderables;
	};
}