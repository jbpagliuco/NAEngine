#include "PhysicsRigidBody.h"

#include "Vendor/PhysX/include/PxPhysics.h"
#include "Vendor/PhysX/include/PxActor.h"
#include "Vendor/PhysX/include/PxRigidDynamic.h"
#include "Vendor/PhysX/include/PxRigidStatic.h"
#include "Vendor/PhysX/include/PxShape.h"
#include "Vendor/PhysX/include/PxScene.h"

#include "Base/Debug/Assert.h"

#include "Physics.h"
#include "PhysicsMaterial.h"
#include "PhysicsShape.h"
#include "PhysicsUtil.h"

namespace na
{
	PhysicsRigidBody::PhysicsRigidBody()
	{
	}

	PhysicsRigidBody::~PhysicsRigidBody()
	{
	}

	bool PhysicsRigidBody::Initialize(const PhysicsRigidBodyInfo &info, const Transform &transform)
	{
		const bool success = CreateShape(info);
		NA_ASSERT_RETURN_VALUE(success, "Failed to create physics shape.", false);

		const physx::PxTransform pxTr = GameTransformToPhysX(transform);

		switch (info.mMotionType)
		{
		case PhysicsMotionType::DYNAMIC:
			mActor = (physx::PxRigidActor*)(PhysicsGetDevice()->createRigidDynamic(pxTr));
			break;

		case PhysicsMotionType::STATIC:
			mActor = (physx::PxRigidActor*)(PhysicsGetDevice()->createRigidStatic(pxTr));
			break;
		};

		NA_ASSERT_RETURN_VALUE(mActor, false, "Failed to create physics actor.");

		mActor->attachShape(*mShape);

		PhysicsGetScene()->addActor(*mActor);

		return true;
	}

	void PhysicsRigidBody::Shutdown()
	{
		PhysicsGetScene()->removeActor(*mActor);

		NA_PHYSICS_SAFE_RELEASE(mActor);
		NA_PHYSICS_SAFE_RELEASE(mShape);
	}

	Transform PhysicsRigidBody::GetTransform()const
	{
		return PhysXTransformToGame(mActor->getGlobalPose());
	}

	bool PhysicsRigidBody::CreateShape(const PhysicsRigidBodyInfo &info)
	{
		switch (info.mShapeInfo.mType)
		{
		case PhysicsShapeType::SPHERE:
		{
			physx::PxSphereGeometry geo;
			geo.radius = info.mShapeInfo.mSphereParams.mRadius;
			NA_ASSERT_RETURN_VALUE(geo.isValid(), "Tried to create an invalid sphere.", false);

			mShape = PhysicsGetDevice()->createShape(geo, *info.mMaterial->GetPhysXMaterial());
			break;
		}

		case PhysicsShapeType::BOX:
		{
			PhysicsBoxInfo params = info.mShapeInfo.mBoxParams;

			physx::PxBoxGeometry geo;
			geo.halfExtents.x = (params.mMax.x - params.mMin.x) / 2.0f;
			geo.halfExtents.y = (params.mMax.y - params.mMin.y) / 2.0f;
			geo.halfExtents.z = (params.mMax.z - params.mMin.z) / 2.0f;
			NA_ASSERT_RETURN_VALUE(geo.isValid(), "Tried to create an invalid box.", false);

			mShape = PhysicsGetDevice()->createShape(geo, *info.mMaterial->GetPhysXMaterial());
			break;
		}
		};

		return mShape != nullptr;
	}
}