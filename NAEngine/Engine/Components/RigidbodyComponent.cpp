#include "RigidbodyComponent.h"

#include "Vendor/PhysX/include/PxPhysics.h"
#include "Vendor/PhysX/include/PxShape.h"
#include "Vendor/PhysX/include/PxScene.h"
#include "Vendor/PhysX/include/PxActor.h"
#include "Vendor/PhysX/include/PxRigidDynamic.h"

#include "Base/Debug/Assert.h"

namespace na
{
	void RigidbodyComponent::Deserialize(DeserializationParameterMap &params)
	{
		const std::string motionType = params["motionType"].AsString();
		if (motionType == "dynamic") {
			mMotionType = PhysicsMotionType::DYNAMIC;
		} else {
			mMotionType = PhysicsMotionType::STATIC;
		}

		const std::string collisionType = params["collisionType"].AsString();
		if (collisionType == "sphere") {
			mShapeInfo.mType = PhysicsShapeType::SPHERE;
			mShapeInfo.mSphereParams.mRadius = params["radius"].AsFloat();
		} else if (collisionType == "box") {
			mShapeInfo.mType = PhysicsShapeType::BOX;
			mShapeInfo.mBoxParams.mMin = params["min"].AsFloat3();
			mShapeInfo.mBoxParams.mMax = params["max"].AsFloat3();
		} else {
			NA_ASSERT(false, "Unknown collision type given to rigidbody component (%s).", collisionType);
		}
	}

	void RigidbodyComponent::Activate()
	{
		mMaterial.Initialize();

		PhysicsRigidBodyInfo info;
		info.mMaterial = &mMaterial;
		info.mMotionType = mMotionType;
		info.mShapeInfo = mShapeInfo;

		mBody.Initialize(info, *mTransform);
	}

	void RigidbodyComponent::Deactivate()
	{
		mBody.Shutdown();
		mMaterial.Shutdown();
	}

	void RigidbodyComponent::UpdatePhysicsPost()
	{
		Transform newTr = mBody.GetTransform();

		mTransform->SetPosition(newTr.CopyPosition());
		mTransform->SetRotation(newTr.CopyRotation());
	}
}