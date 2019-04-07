#pragma once

#include <DirectXMath.h>

namespace na
{
	class Transform
	{
	public:
		Transform();
		Transform(const DirectX::XMFLOAT3 &position, const DirectX::XMVECTOR &qRotation, const DirectX::XMFLOAT3 &scale);

		void ResetPosition();
		DirectX::XMFLOAT3& GetPosition();
		void SetPosition(const DirectX::XMFLOAT3 &position);
		void SetPosition(float x, float y, float z);
		void Translate(const DirectX::XMFLOAT3 &delta);
		void Translate(float x, float y, float z);

		void ResetRotation();
		DirectX::XMVECTOR& GetRotation();
		void SetRotation(const DirectX::XMVECTOR &qRotation);
		void SetRotation(const DirectX::XMFLOAT3 &axis, float angle);
		void SetLookAt(const DirectX::XMFLOAT3 &target);
		void Rotate(float pitch, float yaw, float roll);

		void ResetScale();
		DirectX::XMFLOAT3& GetScale();
		void SetScale(const DirectX::XMFLOAT3 &scale);
		void SetScale(float x, float y, float z);

		const DirectX::XMMATRIX& GetMatrix();

	private:
		DirectX::XMFLOAT3 mPosition;
		DirectX::XMVECTOR mRotation;
		DirectX::XMFLOAT3 mScale;

		DirectX::XMMATRIX mMatrix;

		bool mDirty;

		void Make();
	};

	extern Transform IDENTITY_TRANSFORM;
}