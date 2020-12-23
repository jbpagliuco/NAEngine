#pragma once

#include "RenderableInstance.h"

#include "Base/Streaming/Stream.h"

namespace na
{
	class Texture;

	class BillboardInstance : public RenderableInstance
	{
	public:
		bool Initialize(const Texture *texture);
		void Shutdown();

		virtual void Render(bool bindMaterial = true) override;

	private:
		const Texture *mTexture = nullptr;
	};
}