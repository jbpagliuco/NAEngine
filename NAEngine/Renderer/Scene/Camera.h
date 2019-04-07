#pragma once

#include "Renderer/RenderDefs.h"

#if defined(NA_D3D11)
#include <DirectXMath.h>
#include <d3d11.h>
#endif

namespace na
{
	class Camera
	{
	public:
		DirectX::XMMATRIX mTransform;

		float mFOV;
		float mNear;
		float mFar;
	};
}