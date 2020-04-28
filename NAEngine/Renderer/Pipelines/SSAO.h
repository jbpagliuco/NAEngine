#pragma once

#include "Renderer/Resources/ConstantBuffer.h"
#include "Renderer/Resources/RenderTarget.h"

namespace na
{
	class Camera;
	class Scene;

	class SSAOBuilder
	{
	public:
		bool Initialize();
		void Shutdown();

		void Build(const Scene &scene, const Camera& camera);

	private:
		RenderTarget mNormalDepthRenderTarget;

		ConstantBuffer mPerFrameBuffer;
		ConstantBuffer mPerObjectBuffer;
	};
}