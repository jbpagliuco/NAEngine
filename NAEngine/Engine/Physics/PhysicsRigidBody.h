#pragma once

#include "Base/Math/Transform.h"

#include "PhysicsShape.h"

namespace physx
{
	class PxRigidActor;
	class PxShape;
}

namespace na
{
	class PhysicsMaterial;

	enum class PhysicsMotionType
	{
		STATIC,
		DYNAMIC
	};

	struct PhysicsRigidBodyInfo
	{
		PhysicsMotionType mMotionType;
		PhysicsShapeInfo mShapeInfo;
		PhysicsMaterial *mMaterial;
	};

	class PhysicsRigidBody
	{
	public:
		PhysicsRigidBody();
		virtual ~PhysicsRigidBody();

		bool Initialize(const PhysicsRigidBodyInfo &info, const Transform &transform);
		void Shutdown();

		Transform GetTransform()const;

	private:
		bool CreateShape(const PhysicsRigidBodyInfo &info);

	private:
		physx::PxRigidActor *mActor;
		physx::PxShape *mShape;
	};
}