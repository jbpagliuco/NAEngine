#pragma once

#include "Vendor/PhysX/include/foundation/PxTransform.h"

#include "Base/Math/Transform.h"

namespace na
{
	physx::PxTransform GameTransformToPhysX(const Transform &transform);
	Transform PhysXTransformToGame(const physx::PxTransform &transform);
}