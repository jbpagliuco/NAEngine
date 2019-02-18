#include "RendererD3D.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>

#include "Base/Debug/Assert.h"

#include "Renderer/RenderDefs.h"

namespace na
{
	RendererD3D* RendererD3D::msInst = nullptr;

	bool RendererD3D::Initialize(const RendererInitParams &params)
	{
		NA_FATAL_ERROR(InitDevice(params), "Failed to initialize Direct3D device.");

		return true;
	}

	void RendererD3D::Shutdown()
	{
		if (mSwapChain) {
			mSwapChain->SetFullscreenState(false, nullptr);
		}

		NA_SAFE_RELEASE(mSwapChain);
		NA_SAFE_RELEASE(mContext);
		NA_SAFE_RELEASE(mDevice);
	}

	void RendererD3D::BeginRender()
	{
	}

	void RendererD3D::EndRender()
	{
		mSwapChain->Present(0, 0);
	}

	ID3D11Device* RendererD3D::GetDevice()
	{
		return mDevice;
	}

	ID3D11DeviceContext* RendererD3D::GetContext()
	{
		return mContext;
	}

	IDXGISwapChain* RendererD3D::GetSwapChain()
	{
		return mSwapChain;
	}


	bool RendererD3D::InitDevice(const RendererInitParams &params)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc{};
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferDesc.Width = params.mWidth;
		swapChainDesc.BufferDesc.Height = params.mHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.OutputWindow = params.mWindowHandle;
		swapChainDesc.Windowed = true;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;

		const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
		const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
		const UINT sdkVersion = D3D11_SDK_VERSION;

#if defined(_NA_DEBUG)
		const UINT creationFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
		const UINT creationFlags = 0;
#endif

		HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, creationFlags, &featureLevel, 1, sdkVersion, &swapChainDesc, &mSwapChain, &mDevice, nullptr, &mContext);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to create D3D device/swap chain.");

		return true;
	}
}