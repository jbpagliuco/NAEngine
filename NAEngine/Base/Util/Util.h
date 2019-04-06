#pragma once

#define _COMBINE(x, y) x##y
#define COMBINE(x, y) _COMBINE(x, y)

#define LOW_BYTE(b)		(0x0F & b)
#define HIGH_BYTE(b)	((0xF0 & b) >> 4)
#define LOW_U16(u)		(0x00FF & u)
#define HIGH_U16(u)		((0xFF00 & u) >> 8)
#define LOW_U32(u)		(0x0000FFFF & u)
#define HIGH_U32(u)		((0xFFFF0000 & u) >> 16)
#define LOW_U64(u)		(0x00000000FFFFFFFF & u)
#define HIGH_U64(u)		((0xFFFFFFFF00000000 & u) >> 32)

#define STATIC_ARRAY_SIZE(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

// Will execute once for the entire program
#define EXECUTE_ONCE()																		\
	static bool COMBINE(_execute_once, __LINE__) = true;									\
	if (COMBINE(_execute_once, __LINE__) && !(COMBINE(_execute_once, __LINE__) = false))		


namespace na
{
	template <typename T>
	struct Vector2
	{
		Vector2<T>() {}
		Vector2<T>(T x, T y) : x(x), y(y) {}

		union {
			struct { T x, y; };
			T vArray[2];
		};
	};

	/* Holds 3 components */
	template <typename T>
	struct Vector3
	{
		Vector3<T>() {}
		Vector3<T>(T x, T y, T z) : x(x), y(y), z(z) {}

		union {
			struct { T x, y, z; };
			T vArray[3];
		};
	};

	/* Holds 4 components */
	template <typename T>
	struct Vector4
	{
		Vector4<T>() {}
		Vector4<T>(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

		union {
			struct { T x, y, z, w; };
			T vArray[4];
		};
	};

	typedef Vector2<int> Point;
	typedef Vector2<unsigned int> Size;

	// Returns the larger of v1 and v2
	template <typename T>
	T Max(const T &v1, const T &v2);

	// Returns a reference to the larger of v1 and v2
	template <typename T>
	T& MaxRef(T &v1, T &v2);

	// Returns the smaller of v1 and v2
	template <typename T>
	T Min(const T &v1, const T &v2);

	// Returns a reference to the smaller of v1 and v2
	template <typename T>
	T& MinRef(T &v1, T &v2);

	// Clamps a value to [min, max].
	// @param val - The value to be clamped.
	// @param min - The lower bound of the clamp.
	// @param max - The upper bound of the clamp.
	template <typename T>
	T& Clamp(T& val, const T& min, const T& max);
}

#include "Util.inl"