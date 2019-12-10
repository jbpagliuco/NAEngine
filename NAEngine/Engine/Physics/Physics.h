#pragma once

#include "PhysicsDefs.h"

namespace physx
{
	class PxPhysics;
	class PxScene;
}

namespace na
{
	bool PhysicsSystemInit();
	void PhysicsSystemShutdown();

	void PhysicsSystemDoFrame();

	physx::PxPhysics* PhysicsGetDevice();
	physx::PxScene* PhysicsGetScene();
}