#pragma once

#include "Base/Util/Util.h"

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
		Vector3f mMin;
		Vector3f mMax;
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