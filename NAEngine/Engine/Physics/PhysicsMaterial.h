#pragma once

namespace physx
{
	class PxMaterial;
}

namespace na
{
	class PhysicsMaterial
	{
	public:
		PhysicsMaterial();

		bool Initialize();
		bool Initialize(float staticFriction, float dynamicFriction, float restitution);

		void Shutdown();

		physx::PxMaterial* GetPhysXMaterial()const;
		
	private:
		float mStaticFriction;
		float mDynamicFriction;
		float mRestitution;

		physx::PxMaterial *mMaterial;
	};
}