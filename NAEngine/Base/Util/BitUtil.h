#pragma once

#include <stdint.h>

template <size_t S>
struct _NA_ENUM_FLAG_INTEGER_FOR_SIZE;

template <> struct _NA_ENUM_FLAG_INTEGER_FOR_SIZE<1> { typedef uint8_t type; };
template <> struct _NA_ENUM_FLAG_INTEGER_FOR_SIZE<2> { typedef uint16_t type; };
template <> struct _NA_ENUM_FLAG_INTEGER_FOR_SIZE<4> { typedef uint32_t type; };
template <> struct _NA_ENUM_FLAG_INTEGER_FOR_SIZE<8> { typedef uint64_t type; };

template <class T>
struct _NA_ENUM_FLAG_SIZED_INTEGER
{
	typedef typename _NA_ENUM_FLAG_INTEGER_FOR_SIZE<sizeof(T)>::type type;
};

#define NA_DEFINE_ENUM_FLAG_OPERATORS(ENUMTYPE)																																						\
	inline ENUMTYPE  operator |  (ENUMTYPE a, ENUMTYPE b)   { return (ENUMTYPE)  (((_NA_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a)   |  ((_NA_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); }		\
	inline ENUMTYPE& operator |= (ENUMTYPE &a, ENUMTYPE b)  { return (ENUMTYPE&) (((_NA_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type &)a) |= ((_NA_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); }		\
	inline ENUMTYPE  operator &  (ENUMTYPE a, ENUMTYPE b)   { return (ENUMTYPE)  (((_NA_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a)   &  ((_NA_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); }		\
	inline ENUMTYPE& operator &= (ENUMTYPE &a, ENUMTYPE b)  { return (ENUMTYPE&) (((_NA_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type &)a) &= ((_NA_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); }		\
	inline ENUMTYPE  operator ^  (ENUMTYPE a, ENUMTYPE b)   { return (ENUMTYPE)  (((_NA_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a)   ^  ((_NA_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); }		\
	inline ENUMTYPE& operator ^= (ENUMTYPE &a, ENUMTYPE b)  { return (ENUMTYPE&) (((_NA_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type &)a) ^= ((_NA_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)b)); }		\
	inline ENUMTYPE  operator ~  (ENUMTYPE a)               { return (ENUMTYPE)  (~((_NA_ENUM_FLAG_SIZED_INTEGER<ENUMTYPE>::type)a)); }