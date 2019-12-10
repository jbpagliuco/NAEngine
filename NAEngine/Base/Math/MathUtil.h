#pragma once

namespace na
{
	const float PI = 3.1415927410125732421875;
	const float PI_DIV2 = PI / 2.0f;
	const float PI_DIV3 = PI / 3.0f;
	const float PI_DIV4 = PI / 4.0f;
	const float PI_DIV6 = PI / 6.0f;
	const float PI_MUL2 = PI * 2.0f;
	const float PI_3DIV2 = 3.0f * PI / 2.0f;

	const float FLOAT_EPSILON = 1E-5f;
	const double DOUBLE_EPSILON = 1E-9;
	const long double LONGDOUBLE_EPSILON = DOUBLE_EPSILON;

	// Single precision epsilon-equality.
	// Compares first and second, to see if they equal eachother within epsilon.
	inline bool FEquals(float first, float second, float epsilon = FLOAT_EPSILON) { return (fabsf(first - second) < epsilon); }

	// Double precision epsilon-equality.
	// Compares first and second, to see if they equal eachother within epsilon.
	inline bool DEquals(double first, double second, double epsilon = DOUBLE_EPSILON) { return (fabs(first - second) < epsilon); }

	/* Holds 2 components */
	template <typename T>
	struct Vector2
	{
		Vector2<T>() {}
		Vector2<T>(T x, T y) { X = x; Y = y; }
		T X;
		T Y;
	};

	/* Holds 3 components */
	template <typename T>
	struct Vector3
	{
		Vector3<T>() {}
		Vector3<T>(T x, T y, T z) { X = x; Y = y; Z = z; }
		T X;
		T Y;
		T Z;
	};

	/* Holds 4 components */
	template <typename T>
	struct Vector4
	{
		Vector4<T>() {}
		Vector4<T>(T x, T y, T z, T w) { X = x; Y = y; Z = z; W = w; }
		T X;
		T Y;
		T Z;
		T W;
	};
}