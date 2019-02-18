#pragma once

namespace na
{
	class Scene;

	class SceneRenderer
	{
		virtual void BeginRender();
		virtual void EndRender();

		virtual void RenderScene(Scene *scene);
	};
}