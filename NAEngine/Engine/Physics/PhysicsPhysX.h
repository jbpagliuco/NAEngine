#pragma once

namespace na
{
	bool PhysicsPhysXInit();
	void PhysicsPhysXShutdown();

	void PhysicsPhysXSimulate(float timeStep);
}