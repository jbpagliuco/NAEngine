#include "Physics.h"

#include "PhysicsPhysX.h"

namespace na
{
	bool PhysicsSystemInit()
	{
		return PhysicsPhysXInit();
	}

	void PhysicsSystemShutdown()
	{
		PhysicsPhysXShutdown();
	}
}