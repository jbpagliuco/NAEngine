#pragma once

#include "Base/Debug/Assert.h"

#if defined(_NA_WIN32)
#define NA_D3D11
#else
#undef NA_D3D11
#endif

#define NA_SAFE_RELEASE(x) if (x != nullptr) { x->Release(); x = nullptr; }

#define NA_RENDER_ASSERT_RETURN(cond, ...)				NA_ASSERT_RETURN_CLEANUP(cond, Shutdown, __VA_ARGS__)
#define NA_RENDER_ASSERT_RETURN_VALUE(cond, rv, ...)	NA_ASSERT_RETURN_VALUE_CLEANUP(cond, rv, Shutdown, __VA_ARGS__)