#pragma once

namespace na
{
	class Scene;

	class SceneRenderer
	{
	public:
		virtual bool Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void BeginRender() = 0;
		virtual void EndRender() = 0;

		virtual void RenderScene(Scene *scene) = 0;
	};
}