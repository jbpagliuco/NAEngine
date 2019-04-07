#include "Transform.h"

namespace na
{
	Transform IDENTITY_TRANSFORM;

	Transform::Transform() :
		mDirty(true)
	{
		ResetPosition();
		ResetRotation();
		ResetScale();
	}

	Transform::Transform(const DirectX::XMFLOAT3 &position, const DirectX::XMVECTOR &qRotation, const DirectX::XMFLOAT3 &scale) :
		mDirty(true)
	{
		SetPosition(position);
		SetRotation(qRotation);
		SetScale(scale);
	}

	void Transform::ResetPosition()
	{
		SetPosition(0.0f, 0.0f, 0.0f);
	}

	DirectX::XMFLOAT3& Transform::GetPosition()
	{
		mDirty = true;
		return mPosition;
	}

	void Transform::SetPosition(const DirectX::XMFLOAT3 &position)
	{
		mPosition = position;
		mDirty = true;
	}

	void Transform::SetPosition(float x, float y, float z)
	{
		mPosition = DirectX::XMFLOAT3(x, y, z);
		mDirty = true;
	}

	void Transform::Translate(const DirectX::XMFLOAT3 &delta)
	{
		mPosition.x += delta.x;
		mPosition.y += delta.y;
		mPosition.z += delta.z;

		mDirty = true;
	}

	void Transform::Translate(float x, float y, float z)
	{
		mPosition.x += x;
		mPosition.y += y;
		mPosition.z += z;

		mDirty = true;
	}

	void Transform::ResetRotation()
	{
		mRotation = DirectX::XMQuaternionIdentity();
		mDirty = true;
	}

	DirectX::XMVECTOR& Transform::GetRotation()
	{
		mDirty = true;
		return mRotation;
	}

	void Transform::SetRotation(const DirectX::XMVECTOR &qRotation)
	{
		mRotation = qRotation;
		mDirty = true;
	}

	void Transform::SetRotation(const DirectX::XMFLOAT3 &axis, float angle)
	{
		const DirectX::XMFLOAT4 axisW(axis.x, axis.y, axis.z, 0.0f);
		DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat4(&axisW), angle);

		mDirty = true;
	}

	void Transform::SetLookAt(const DirectX::XMFLOAT3 &target)
	{
		using namespace DirectX;

		XMMATRIX m = XMMatrixLookAtLH(XMLoadFloat3(&mPosition), XMLoadFloat3(&target), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		mRotation = XMQuaternionRotationMatrix(XMMatrixTranspose(m));

		mDirty = true;
	}

	void Transform::ResetScale()
	{
		SetScale(1.0f, 1.0f, 1.0f);
		mDirty = true;
	}

	DirectX::XMFLOAT3& Transform::GetScale()
	{
		mDirty = true;
		return mScale;
	}

	void Transform::SetScale(const DirectX::XMFLOAT3 &scale)
	{
		mScale = scale;
		mDirty = true;
	}

	void Transform::SetScale(float x, float y, float z)
	{
		mScale = DirectX::XMFLOAT3(x, y, z);
		mDirty = true;
	}

	const DirectX::XMMATRIX& Transform::GetMatrix()
	{
		if (mDirty) {
			Make();
			mDirty = false;
		}

		return mMatrix;
	}

	void Transform::Make()
	{
		const DirectX::XMVECTOR zero = DirectX::XMVectorZero();
		mMatrix = DirectX::XMMatrixTransformation(zero, zero, DirectX::XMLoadFloat3(&mScale),
			zero, mRotation,
			DirectX::XMLoadFloat3(&mPosition));
	}
}