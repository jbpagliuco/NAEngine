#pragma once

#include "Base/DataStructures/Singleton.h"
#include "Base/OS/OS.h"

#include "RenderDefs.h"
#include "StateData.h"

namespace na
{
	struct RendererInitParams
	{
		Window mWindow;
		int mWidth;
		int mHeight;
	};

	class Renderer : public Singleton<Renderer>
	{
	public:
		virtual bool Initialize(const RendererInitParams &params);
		virtual void Shutdown() = 0;

		virtual void BeginRender() = 0;
		virtual void EndRender() = 0;

		Window GetWindow() { return mWindow; }

		StateData* GetStateData() { return &mStateData; }

	protected:
		Window mWindow;
		StateData mStateData;
	};
}