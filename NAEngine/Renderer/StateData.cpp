#include "StateData.h"

#include <d3d11.h>

#include "Resources/IndexBuffer.h"
#include "Resources/VertexBuffer.h"

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
		Matrix world;
		Matrix worldInverseTranspose;
	};
	



	bool StateData::Initialize()
	{
		bool success = mViewProjBuffer.Initialize(BufferUsage::DYNAMIC, nullptr, sizeof(Matrix));
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

	void StateData::SetViewProjMatrices(const Matrix &view, const Matrix &proj)
	{
		Matrix viewProj = proj * view;
		mViewProjBuffer.Map(&viewProj);

		PlatformConstantBuffer *cb = mViewProjBuffer.GetBuffer();
		NA_RContext->VSSetConstantBuffers((int)VSConstantBuffers::VIEWPROJ, 1, &cb);
	}

	void StateData::SetObjectTransform(const Matrix &transform)
	{
		PerObjectData data;
		data.world = transform;
		data.worldInverseTranspose = transform.Inverted().Transposed();

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

	void StateData::BindIndexBuffer(const IndexBuffer &ib)
	{
		mCommandContext.BindIndexBuffer(ib.GetBuffer(), NGAIndexBufferType::IBT_32BIT);
	}

	void StateData::BindVertexBuffer(const VertexBuffer &vb)
	{
		mCommandContext.BindVertexBuffer(vb.GetBuffer(), vb.GetVertexStride());
	}

	void StateData::BindShaderResource(const NGAShaderResourceView &view, NGAShaderStage stage, int slot)
	{
		mCommandContext.BindShaderResource(view, stage, slot);
	}

	void StateData::BindSamplerState(const NGASamplerState &samplerState, NGAShaderStage stage, int slot)
	{
		mCommandContext.BindSamplerState(samplerState, stage, slot);
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