#pragma once

#include "Renderer/Renderer.h"

#if defined(NA_D3D11)

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

#if defined(NA_D3D11)
#define NA_Renderer ((na::RendererD3D*)na::RendererD3D::Get())
#define NA_RDevice NA_Renderer->GetDevice()
#define NA_RContext NA_Renderer->GetContext()
#define NA_RSwapChain NA_Renderer->GetSwapChain()
#endif

namespace na
{
	class RendererD3D : public Renderer
	{
	public:
		bool Initialize(const RendererInitParams &params);
		void Shutdown();

		void BeginRender();
		void EndRender();

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContext();
		IDXGISwapChain* GetSwapChain();

	private:
		bool InitDevice(const RendererInitParams &params);

		ID3D11Device *mDevice;
		ID3D11DeviceContext* mContext;
		IDXGISwapChain* mSwapChain;
	};
}

#endif