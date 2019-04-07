#include "Transform.h"

namespace na
{
	using namespace DirectX;

	Transform IDENTITY_TRANSFORM;

	Transform::Transform() :
		mDirty(true)
	{
		ResetPosition();
		ResetRotation();
		ResetScale();
	}

	Transform::Transform(const XMFLOAT3 &position, const XMVECTOR &qRotation, const XMFLOAT3 &scale) :
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

	XMFLOAT3& Transform::GetPosition()
	{
		mDirty = true;
		return mPosition;
	}

	void Transform::SetPosition(const XMFLOAT3 &position)
	{
		mPosition = position;
		mDirty = true;
	}

	void Transform::SetPosition(float x, float y, float z)
	{
		mPosition = XMFLOAT3(x, y, z);
		mDirty = true;
	}

	void Transform::Translate(const XMFLOAT3 &delta)
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
		mRotation = XMQuaternionIdentity();
		mDirty = true;
	}

	XMVECTOR& Transform::GetRotation()
	{
		mDirty = true;
		return mRotation;
	}

	void Transform::SetRotation(const XMVECTOR &qRotation)
	{
		mRotation = qRotation;
		mDirty = true;
	}

	void Transform::SetRotation(const XMFLOAT3 &axis, float angle)
	{
		const XMFLOAT4 axisW(axis.x, axis.y, axis.z, 0.0f);
		XMQuaternionRotationAxis(XMLoadFloat4(&axisW), angle);

		mDirty = true;
	}

	void Transform::SetLookAt(const XMFLOAT3 &target)
	{
		XMMATRIX m = XMMatrixLookAtLH(XMLoadFloat3(&mPosition), XMLoadFloat3(&target), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		mRotation = XMQuaternionRotationMatrix(XMMatrixTranspose(m));

		mDirty = true;
	}

	void Transform::Rotate(float pitch, float yaw, float roll)
	{
		XMVECTOR rot = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
		mRotation = XMQuaternionMultiply(mRotation, rot);

		mDirty = true;
	}

	void Transform::ResetScale()
	{
		SetScale(1.0f, 1.0f, 1.0f);
	}

	XMFLOAT3& Transform::GetScale()
	{
		mDirty = true;
		return mScale;
	}

	void Transform::SetScale(const XMFLOAT3 &scale)
	{
		mScale = scale;
		mDirty = true;
	}

	void Transform::SetScale(float x, float y, float z)
	{
		mScale = XMFLOAT3(x, y, z);
		mDirty = true;
	}

	const XMMATRIX& Transform::GetMatrix()
	{
		if (mDirty) {
			Make();
		}

		return mMatrix;
	}

	void Transform::Make()
	{
		const XMVECTOR zero = XMVectorZero();
		mMatrix = XMMatrixTransformation(zero, zero, XMLoadFloat3(&mScale),
			zero, mRotation,
			XMLoadFloat3(&mPosition));

		mDirty = false;
	}
}