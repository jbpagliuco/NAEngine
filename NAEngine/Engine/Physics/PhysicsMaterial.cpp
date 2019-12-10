#include "PhysicsMaterial.h"

#include "Vendor/PhysX/include/PxMaterial.h"
#include "Vendor/PhysX/include/PxPhysics.h"

#include "Base/Debug/Assert.h"
#include "Physics.h"

namespace na
{
	static constexpr float DEFAULT_STATIC_FRICTION = 0.5f;
	static constexpr float DEFAULT_DYNAMIC_FRICTION = 0.5f;
	static constexpr float DEFAULT_RESTITUTION = 0.5f;

	PhysicsMaterial::PhysicsMaterial() :
		mStaticFriction(0.0f),
		mDynamicFriction(0.0f),
		mRestitution(0.0f),
		mMaterial(nullptr)
	{
	}

	bool PhysicsMaterial::Initialize()
	{
		return Initialize(DEFAULT_STATIC_FRICTION, DEFAULT_DYNAMIC_FRICTION, DEFAULT_RESTITUTION);
	}

	bool PhysicsMaterial::Initialize(float staticFriction, float dynamicFriction, float restitution)
	{
		mStaticFriction = staticFriction;
		mDynamicFriction = dynamicFriction;
		mRestitution = restitution;

		mMaterial = PhysicsGetDevice()->createMaterial(staticFriction, dynamicFriction, restitution);
		NA_ASSERT_RETURN_VALUE(mMaterial != nullptr, false, "Failed to create physics material.");

		return true;
	}
	
	void PhysicsMaterial::Shutdown()
	{
		NA_PHYSICS_SAFE_RELEASE(mMaterial);
	}

	physx::PxMaterial* PhysicsMaterial::GetPhysXMaterial()const
	{
		return mMaterial;
	}
}