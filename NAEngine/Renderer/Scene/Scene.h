#pragma once

#include <vector>

namespace na
{
	class RenderableInstance;

	class Scene
	{
	public:
		static Scene *GetInstance();

		void AddRenderable(RenderableInstance *r);

		const std::vector<RenderableInstance*>& GetRenderables()const;

	private:
		std::vector<RenderableInstance*> mRenderables;
	};
}