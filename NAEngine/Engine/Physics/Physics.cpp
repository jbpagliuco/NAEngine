#include "Physics.h"

#include "Base/Debug/Assert.h"
#include "Base/Util/Timer.h"

#include "Engine/Components/GameComponent.h"
#include "Engine/Physics/PhysicsErrorHandler.h"
#include "Engine/Physics/PhysicsMemory.h"

namespace na
{
	////////////////////////////////
	// Variables
	////////////////////////////////
	static constexpr float PHYSICS_TIME_STEP = 1.0f / 60.0f;
	static Timer FrameTimer;

	static PhysicsAllocator Allocator;
	static PhysicsErrorHandler ErrorHandler;

	static physx::PxFoundation *PXFoundation;
	static physx::PxPhysics *PXPhysics;

	static physx::PxDefaultCpuDispatcher *PXCPUDispatcher;
	static physx::PxScene *PXScene;

	////////////////////////////////
	// Internal Functions
	////////////////////////////////
	static void PhysicsSimulate(float timeStep)
	{
		PXScene->simulate(timeStep);
		PXScene->fetchResults(true);
	}

	////////////////////////////////
	// External Functions
	////////////////////////////////
	bool PhysicsSystemInit()
	{
		FrameTimer.Start((int)(PHYSICS_TIME_STEP * 1000.0f));

		PXFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, Allocator, ErrorHandler);
		NA_FATAL_ERROR(PXFoundation, "PxCreateFoundation failed!");

		PXPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *PXFoundation, physx::PxTolerancesScale(), true, nullptr);
		NA_FATAL_ERROR(PXPhysics, "PxCreatePhysics failed!");

		PXCPUDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		NA_FATAL_ERROR(PXCPUDispatcher, "Failed to create PhysX CPU Dispatcher!");

		physx::PxSceneDesc sceneDesc(PXPhysics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		sceneDesc.cpuDispatcher = PXCPUDispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		PXScene = PXPhysics->createScene(sceneDesc);
		NA_FATAL_ERROR(PXScene, "Failed to create PhysX scene!");

		return true;
	}

	void PhysicsSystemShutdown()
	{
		NA_PHYSICS_SAFE_RELEASE(PXCPUDispatcher);
		NA_PHYSICS_SAFE_RELEASE(PXScene);

		NA_PHYSICS_SAFE_RELEASE(PXPhysics);
		NA_PHYSICS_SAFE_RELEASE(PXFoundation);
	}

	void PhysicsSystemDoFrame()
	{
		if (FrameTimer.Elapsed()) {
			// Run simulations
			PhysicsSimulate(PHYSICS_TIME_STEP);

			// Update components
			GameComponentPhysicsPost();

			FrameTimer.Start();
		}
	}

	physx::PxPhysics* PhysicsGetDevice()
	{
		return PXPhysics;
	}

	physx::PxScene* PhysicsGetScene()
	{
		return PXScene;
	}
}