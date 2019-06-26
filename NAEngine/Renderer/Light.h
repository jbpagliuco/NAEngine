#pragma once

#include <stdint.h>

#include <DirectXMath.h>

namespace na
{
	enum class LightType
	{
		DIRECTIONAL = 0,
		POINT = 1,
		SPOT = 2
	};

	struct Light
	{
		int32_t mType;
		int32_t mEnabled;
		float mIntensity;
		int32_t _padding0;

		DirectX::XMFLOAT3 mPosition;
		float _padding1;

		DirectX::XMFLOAT3 mDirection;
		float mRadius;

		DirectX::XMFLOAT4 mColor;

		float mSpotLightAngle;
		float mConstantAttenuation;
		float mLinearAttenuation;
		float mQuadraticAttenuation;
	};

	Light* CreateLight(LightType type);
}