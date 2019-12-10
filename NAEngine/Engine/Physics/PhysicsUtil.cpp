#include "PhysicsUtil.h"

namespace na
{
	physx::PxTransform GameTransformToPhysX(const Transform &transform)
	{
		DirectX::XMFLOAT3 pos = transform.CopyPosition();

		DirectX::XMVECTOR axis;
		float angle;
		DirectX::XMQuaternionToAxisAngle(&axis, &angle, transform.CopyRotation());
		axis = DirectX::XMVector3Normalize(axis);

		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, axis);

		return physx::PxTransform(
			physx::PxVec3(pos.x, pos.y, pos.z),
			physx::PxQuat(angle, physx::PxVec3(rot.x, rot.y, rot.z))
		);
	}

	Transform PhysXTransformToGame(const physx::PxTransform &transform)
	{
		const DirectX::XMFLOAT4 rotQ(transform.q.x, transform.q.y, transform.q.z, transform.q.w);
		return Transform(
			DirectX::XMFLOAT3(transform.p.x, transform.p.y, transform.p.z),
			DirectX::XMLoadFloat4(&rotQ),
			DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)
		);
	}
}