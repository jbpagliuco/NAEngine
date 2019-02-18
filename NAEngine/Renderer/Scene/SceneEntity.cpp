#include "SceneEntity.h"

namespace na
{
	void SceneEntity::SetPosition(const DirectX::XMFLOAT3 &position)
	{
		mDirty = true;
		mPosition = position;
	}

	DirectX::XMFLOAT3 SceneEntity::GetPosition()const
	{
		return mPosition;
	}


	void SceneEntity::SetRotation(const DirectX::XMVECTOR &rotation)
	{
		mDirty = true;
		mRotation = rotation;
	}

	DirectX::XMVECTOR SceneEntity::GetRotation()const
	{
		return mRotation;
	}


	void SceneEntity::SetWorldTransform(const DirectX::XMMATRIX &worldTransform)
	{
		mDirty = false;
		mWorldTransform = worldTransform;
	}

	DirectX::XMMATRIX SceneEntity::GetWorldTransform()const
	{
		return mWorldTransform;
	}


	void SceneEntity::CalculateWorldTransform()
	{
		if (mDirty) {
			static const DirectX::XMVECTOR zero = DirectX::XMVectorZero();
			static const DirectX::XMVECTOR one = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
			mWorldTransform = DirectX::XMMatrixTransformation(
				zero, zero, one,								// scale
				zero, mRotation,								// rotation
				DirectX::XMLoadFloat3(&mPosition)				// position
			);

			mDirty = false;
		}
	}
}