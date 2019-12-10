#pragma once

#include "Vector.h"

namespace na
{
	class Matrix;

	// Represents a 4D unit quaternion vector used for rotations.
	class Quaternion
	{
	public:
		Quaternion();
		// Creates a quaternion, where xAxis, yAxis, and zAxis make up the unit vector to 
		// rotate about, and angleInRadians is the amount to rotate.
		Quaternion(float xAxis, float yAxis, float zAxis, float angleInRadians);
		// Creates a quaternion, where axis is the unit vector to rotate about, and
		// angleInRadians is the amount to rotate.
		Quaternion(const Vector3f &axis, float angleInRadians);


		static Quaternion FromEuler(float x, float y, float z);
		static Quaternion FromMatrix(const Matrix &mat);

	public:
		Vector3f ToEuler()const;

		// Inverts this quaternion.
		// Returns a reference to this.
		Quaternion& Invert();

		// Multiplies the two quaternions and returns the result.
		Quaternion operator*(const Quaternion &rhs)const;
		// Multiplies the two quaternions and sets the result equal to this.
		// Returns a reference to this.
		Quaternion& operator*=(const Quaternion &rhs);

		// Rotates a vector about the quaternion and returns the resulting rotated vector.
		Vector3f operator*(const Vector3f &rhs)const;

		// Linearly interpolates two quaternions by the amount t and returns the normalized quaternion.
		Quaternion Lerp(const Quaternion &other, float t)const;
		// Spherically linearly interpolates tw quaternions by the amount t and returns the quaternion.
		Quaternion Slerp(const Quaternion &other, float t)const;

	private:
		Quaternion Interpolate(const Quaternion &q1, const Quaternion &q2, float weightA, float weightB)const;

	public:
		float x;
		float y;
		float z;
		float w;
	};
}