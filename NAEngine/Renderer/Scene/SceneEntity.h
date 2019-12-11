#pragma once

#include "Base/Math/Vector.h"
#include "Base/Math/Matrix.h"
#include "Base/Math/Quaternion.h"

namespace na
{
	class SceneEntity
	{
	public:
		SceneEntity();

		void SetPosition(const Vector3f &position);
		Vector3f GetPosition()const;

		void SetRotation(const Quaternion &rotation);
		Quaternion GetRotation()const;

		void SetWorldTransform(const Matrix &worldTransform);
		Matrix GetWorldTransform();

	private:
		Vector3f mPosition;
		Quaternion mRotation;

		bool mDirty;
		Matrix mWorldTransform;
	};
}