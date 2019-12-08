#include "Physics.h"

#include "Base/Util/Timer.h"

#include "PhysicsPhysX.h"

namespace na
{
	static constexpr float PHYSICS_TIME_STEP = 1.0f / 60.0f;
	static Timer FrameTimer;

	bool PhysicsSystemInit()
	{
		FrameTimer.Start((int)(PHYSICS_TIME_STEP * 1000.0f));

		return PhysicsPhysXInit();
	}

	void PhysicsSystemShutdown()
	{
		PhysicsPhysXShutdown();
	}

	void PhysicsSystemDoFrame()
	{
		if (FrameTimer.Elapsed()) {
			// Run simulations
			PhysicsPhysXSimulate(PHYSICS_TIME_STEP);

			FrameTimer.Start();
		}
	}
}