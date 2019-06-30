#include "StateData.h"

#include <d3d11.h>
#include <DirectXMath.h>

#include "Rect.h"
#include "RendererD3D.h"

#include "Light.h"

namespace na
{
	StateData RendererStateData;

	enum class VSConstantBuffers
	{
		VIEWPROJ = 0,
		OBJECTDATA,
		USER
	};

	enum class PSConstantBuffers
	{
		LIGHTS = 0,
		USER
	};

	struct PerObjectData
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX worldInverseTranspose;
	};
	



	bool StateData::Initialize()
	{
		bool success = mViewProjBuffer.Initialize(BufferUsage::DYNAMIC, nullptr, sizeof(DirectX::XMMATRIX));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize view proj buffer.");

		success = mObjectDataBuffer.Initialize(BufferUsage::DYNAMIC, nullptr, sizeof(PerObjectData));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize object data buffer.");

		success = mLightsBuffer.Initialize(BufferUsage::DYNAMIC, nullptr, sizeof(LightsData));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize lights buffer.");
		
		mRasterizerState = nullptr;

		return true;
	}

	void StateData::Shutdown()
	{
		mViewProjBuffer.Shutdown();
		mObjectDataBuffer.Shutdown();
		mLightsBuffer.Shutdown();

		NA_SAFE_RELEASE(mRasterizerState);
	}

	void StateData::SetViewport(const Rect &rect)
	{
		D3D11_VIEWPORT vp;
		vp.TopLeftX = rect.x;
		vp.TopLeftY = rect.y;
		vp.Width = rect.w;
		vp.Height = rect.h;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		NA_RContext->RSSetViewports(1, &vp);
	}

	void StateData::SetViewProjMatrices(const DirectX::XMMATRIX &view, const DirectX::XMMATRIX &proj)
	{
		DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiplyTranspose(view, proj);
		mViewProjBuffer.Map(&viewProj);

		PlatformConstantBuffer *cb = mViewProjBuffer.GetBuffer();
		NA_RContext->VSSetConstantBuffers((int)VSConstantBuffers::VIEWPROJ, 1, &cb);
	}

	void StateData::SetObjectTransform(const DirectX::XMMATRIX &transform)
	{
		PerObjectData data;
		data.world = DirectX::XMMatrixTranspose(transform);
		data.worldInverseTranspose = DirectX::XMMatrixInverse(nullptr, transform); // ^T ^T

		mObjectDataBuffer.Map(&data);

		PlatformConstantBuffer *cb = mObjectDataBuffer.GetBuffer();
		NA_RContext->VSSetConstantBuffers((int)VSConstantBuffers::OBJECTDATA, 1, &cb);
	}

	void StateData::SetLightsData(const LightsData &lights)
	{
		mLightsBuffer.Map((void*)&lights);

		PlatformConstantBuffer *cb = mLightsBuffer.GetBuffer();
		NA_RContext->PSSetConstantBuffers((int)PSConstantBuffers::LIGHTS, 1, &cb);
	}

	void StateData::SetRasterizerState()
	{
		if (mRasterizerState == nullptr) {
			D3D11_RASTERIZER_DESC desc{};
			desc.AntialiasedLineEnable = false;
			desc.CullMode = D3D11_CULL_BACK;
			desc.DepthBias = 0;
			desc.DepthBiasClamp = 0.0f;
			desc.DepthClipEnable = true;
			desc.FillMode = D3D11_FILL_SOLID;
			desc.FrontCounterClockwise = false;
			desc.MultisampleEnable = false;
			desc.ScissorEnable = false;
			desc.SlopeScaledDepthBias = 0.0f;

			HRESULT hr = NA_RDevice->CreateRasterizerState(&desc, &mRasterizerState);
			NA_ASSERT(SUCCEEDED(hr), "Failed to create rasterizer state.");
		}

		NA_RContext->RSSetState(mRasterizerState);
	}

	int StateData::GetUserVSConstantBufferIndex()const
	{
		return (int)VSConstantBuffers::USER;
	}

	int StateData::GetUserPSConstantBufferIndex()const
	{
		return (int)PSConstantBuffers::USER;
	}
}