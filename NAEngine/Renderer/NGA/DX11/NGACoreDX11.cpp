#pragma once

#include "NGA/NGACore.h"

#if defined(NGA_D3D11)

#include "Base/Debug/Assert.h"

#include "NGACoreInternalDX11.h"

namespace na
{
	void NGAInitialize(const NGAInitParams &params)
	{
		constexpr D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
		constexpr D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
		constexpr UINT sdkVersion = D3D11_SDK_VERSION;

#if defined(_NA_DEBUG)
		constexpr UINT creationFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
		constexpr UINT creationFlags = 0;
#endif

		HRESULT hr = D3D11CreateDevice(nullptr, driverType, nullptr, creationFlags, &featureLevel, 1, sdkVersion, &NgaDx11State.mDevice, nullptr, &NgaDx11State.mContext);
		NA_FATAL_ERROR(SUCCEEDED(hr), "Failed to create DirectX 11 device.");
	}

	void NGAShutdown()
	{
		NGA_DX11_SAFE_RELEASE(NgaDx11State.mDevice);
		NGA_DX11_SAFE_RELEASE(NgaDx11State.mContext);
	}
}

#endif // defined(NGA_D3D11)