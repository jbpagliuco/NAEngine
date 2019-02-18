#pragma once

#include "Base/DataStructures/Singleton.h"
#include "Base/OS/OS.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

namespace na
{
	struct RendererInitParams
	{
		WindowHandle mWindowHandle;
		int mWidth;
		int mHeight;
	};

	class RendererD3D : public Singleton<RendererD3D>
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