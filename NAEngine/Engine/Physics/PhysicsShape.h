#pragma once

#include "Base/Math/Transform.h"

namespace na
{
	enum class PhysicsShapeType
	{
		SPHERE = 0,
		BOX
	};

	struct PhysicsSphereInfo
	{
		float mRadius;
	};

	struct PhysicsBoxInfo
	{
		DirectX::XMFLOAT3 mMin;
		DirectX::XMFLOAT3 mMax;
	};

	struct PhysicsShapeInfo
	{
		PhysicsShapeType mType;

		union
		{
			PhysicsSphereInfo mSphereParams;
			PhysicsBoxInfo mBoxParams;
		};
	};
}