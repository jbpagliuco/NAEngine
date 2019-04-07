#pragma once

#include <DirectXMath.h>

namespace na
{
	class SceneEntity
	{
	public:
		SceneEntity();

		void SetPosition(const DirectX::XMFLOAT3 &position);
		DirectX::XMFLOAT3 GetPosition()const;

		void SetRotation(const DirectX::XMVECTOR &rotation);
		DirectX::XMVECTOR GetRotation()const;

		void SetWorldTransform(const DirectX::XMMATRIX &worldTransform);
		DirectX::XMMATRIX GetWorldTransform()const;

	private:
		void CalculateWorldTransform();

		DirectX::XMFLOAT3 mPosition;
		DirectX::XMVECTOR mRotation;

		bool mDirty;
		DirectX::XMMATRIX mWorldTransform;
	};
}