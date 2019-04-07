#include "Renderer.h"

#include "Rect.h"

namespace na
{
	bool Renderer::Initialize(const RendererInitParams &params)
	{
		mWindow = params.mWindow;
		
		if (!mStateData.Initialize()) {
			return false;
		}

		return true;
	}

	void Renderer::Shutdown()
	{
		mStateData.Shutdown();
	}

	void Renderer::BeginRender()
	{
		Rect r;
		r.x = 0.0f;
		r.y = 0.0f;
		r.w = (float)mWindow.width;
		r.h = (float)mWindow.height;

		mStateData.SetViewport(r);
		mStateData.SetRasterizerState();
	}
}