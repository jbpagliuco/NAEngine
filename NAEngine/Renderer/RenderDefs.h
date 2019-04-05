#pragma once

#include "Base/Debug/Assert.h"

#if defined(_NA_WIN32)
#define NA_D3D11
#else
#undef NA_D3D11
#endif

#define NA_SAFE_RELEASE(x) if (x != nullptr) { x->Release(); x = nullptr; }