#pragma once

#include "Base/DataStructures/Singleton.h"

namespace na
{
	struct RendererInitParams
	{

	};

	class RendererD3D : public Singleton<RendererD3D>
	{
	public:
		bool Initialize(const RendererInitParams &params);
		void Shutdown();

		void BeginRender();
		void EndRender();
	};
}