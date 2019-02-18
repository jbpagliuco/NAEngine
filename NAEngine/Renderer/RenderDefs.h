#pragma once

#if defined(_NA_WIN32)
#define NA_D3D
#include "D3D/RendererD3D.h"
#else
#undef NA_D3D
#endif

#define NA_Renderer RendererD3D::Get()
#define NA_RDevice NA_Renderer->GetDevice()
#define NA_RContext NA_Renderer->GetContext()

#define NA_SAFE_RELEASE(x) if (x != nullptr) { x->Release(); x = nullptr; }