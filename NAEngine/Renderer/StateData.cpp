#include "StateData.h"

#include <d3d11.h>

#include "Rect.h"
#include "RendererD3D.h"

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
		USER = 0
	};

	struct PerObjectData
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX worldInverseTranspose;
	};

	bool StateData::Initialize()
	{
		{
			// ViewProj buffer
			D3D11_BUFFER_DESC desc;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = sizeof(DirectX::XMMATRIX);
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			HRESULT hr = NA_RDevice->CreateBuffer(&desc, nullptr, &mViewProjBuffer);
			NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to create view proj buffer");
		}

		{
			// Object buffer
			D3D11_BUFFER_DESC desc;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = sizeof(PerObjectData);
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;

			HRESULT hr = NA_RDevice->CreateBuffer(&desc, nullptr, &mObjectDataBuffer);
			NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to create object data buffer");
		}

		mRasterizerState = nullptr;

		return true;
	}

	void StateData::Shutdown()
	{
		NA_SAFE_RELEASE(mViewProjBuffer);
		NA_SAFE_RELEASE(mObjectDataBuffer);
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
		D3D11_MAPPED_SUBRESOURCE res;

		HRESULT hr = NA_RContext->Map(mViewProjBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
		NA_ASSERT_RETURN(SUCCEEDED(hr));

		DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiplyTranspose(view, proj);
		memcpy(res.pData, &viewProj, sizeof(DirectX::XMMATRIX));

		NA_RContext->Unmap(mViewProjBuffer, 0);

		NA_RContext->VSSetConstantBuffers((int)VSConstantBuffers::VIEWPROJ, 1, &mViewProjBuffer);
	}

	void StateData::SetObjectTransform(const DirectX::XMMATRIX &transform)
	{
		D3D11_MAPPED_SUBRESOURCE res;

		HRESULT hr = NA_RContext->Map(mObjectDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
		NA_ASSERT_RETURN(SUCCEEDED(hr));

		PerObjectData data;
		data.world = DirectX::XMMatrixTranspose(transform);
		data.worldInverseTranspose = DirectX::XMMatrixInverse(nullptr, transform); // ^T ^T
		memcpy(res.pData, &data, sizeof(PerObjectData));

		NA_RContext->Unmap(mObjectDataBuffer, 0);

		NA_RContext->VSSetConstantBuffers((int)VSConstantBuffers::OBJECTDATA, 1, &mObjectDataBuffer);
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