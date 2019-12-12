#pragma once

#include <stdint.h>

#include "Base/Util/Util.h"

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

		Tuple3<float> mPosition;
		float _padding1;

		Tuple3<float> mDirection;
		float mRadius;

		Tuple4<float> mColor;

		float mSpotLightAngle;
		float mConstantAttenuation;
		float mLinearAttenuation;
		float mQuadraticAttenuation;
	};

	Light* CreateLight(LightType type);
	void DestroyLight(Light *pLight);
}