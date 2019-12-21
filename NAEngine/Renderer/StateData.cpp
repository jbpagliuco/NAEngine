#include "StateData.h"

#include <d3d11.h>

#include "Resources/IndexBuffer.h"
#include "Resources/VertexBuffer.h"
#include "Shader/ShaderProgram.h"

#include "Rect.h"
#include "Renderer.h"

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
		bool success = mViewProjBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(Matrix));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize view proj buffer.");

		success = mObjectDataBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(PerObjectData));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize object data buffer.");

		success = mLightsBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(LightsData));
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
		NGAViewport vp;
		vp.mX = rect.x;
		vp.mY = rect.y;
		vp.mWidth = rect.w;
		vp.mHeight = rect.h;
		vp.mMinDepth = 0.0f;
		vp.mMaxDepth = 1.0f;

		mCommandContext.SetViewport(vp);
	}

	void StateData::SetViewProjMatrices(const Matrix &view, const Matrix &proj)
	{
		Matrix viewProj = proj * view;
		mViewProjBuffer.Map(&viewProj);

		mCommandContext.BindConstantBuffer(mViewProjBuffer.GetBuffer(), NGA_SHADER_STAGE_VERTEX, (int)VSConstantBuffers::VIEWPROJ);
	}

	void StateData::SetObjectTransform(const Matrix &transform)
	{
		PerObjectData data;
		data.world = transform;
		data.worldInverseTranspose = transform.Inverted().Transposed();

		mObjectDataBuffer.Map(&data);

		mCommandContext.BindConstantBuffer(mObjectDataBuffer.GetBuffer(), NGA_SHADER_STAGE_VERTEX, (int)VSConstantBuffers::OBJECTDATA);
	}

	void StateData::SetLightsData(const LightsData &lights)
	{
		mLightsBuffer.Map((void*)&lights);

		mCommandContext.BindConstantBuffer(mLightsBuffer.GetBuffer(), NGA_SHADER_STAGE_PIXEL, (int)PSConstantBuffers::LIGHTS);
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

	void StateData::SetPrimitiveTopology(NGAPrimitiveTopology primTopology)
	{
		mCommandContext.SetPrimitiveTopology(primTopology);
	}

	void StateData::BindIndexBuffer(const IndexBuffer &ib)
	{
		mCommandContext.BindIndexBuffer(ib.GetBuffer(), NGAIndexBufferType::IBT_32BIT);
	}

	void StateData::BindVertexBuffer(const VertexBuffer &vb)
	{
		mCommandContext.BindVertexBuffer(vb.GetBuffer(), vb.GetVertexStride());
	}

	void StateData::BindInputLayout(const NGAInputLayout &inputLayout)
	{
		mCommandContext.BindInputLayout(inputLayout);
	}

	void StateData::BindShader(const ShaderProgram &shader)
	{
		mCommandContext.BindShader(shader.GetShader());
	}

	void StateData::BindShaderResource(const NGAShaderResourceView &view, NGAShaderStage stage, int slot)
	{
		mCommandContext.BindShaderResource(view, stage, slot);
	}

	void StateData::BindConstantBuffer(const NGABuffer &constantBuffer, NGAShaderStage stage, int slot)
	{
		NA_ASSERT_RETURN(stage != NGA_SHADER_STAGE_ALL, "Need to implement this.");

		if (stage & NGA_SHADER_STAGE_VERTEX) {
			mCommandContext.BindConstantBuffer(constantBuffer, NGA_SHADER_STAGE_VERTEX, slot + (int)VSConstantBuffers::USER);
		}

		if (stage & NGA_SHADER_STAGE_PIXEL) {
			mCommandContext.BindConstantBuffer(constantBuffer, NGA_SHADER_STAGE_PIXEL, slot + (int)PSConstantBuffers::USER);
		}
	}

	void StateData::BindSamplerState(const NGASamplerState &samplerState, NGAShaderStage stage, int slot)
	{
		mCommandContext.BindSamplerState(samplerState, stage, slot);
	}

	void StateData::ClearRenderTarget(const NGARenderTargetView &renderTargetView, const float *clearColor)
	{
		mCommandContext.ClearRenderTarget(renderTargetView, clearColor);
	}

	void StateData::ClearDepthStencilView(const NGADepthStencilView &depthStencilView)
	{
		mCommandContext.ClearDepthStencilView(depthStencilView);
	}

	void StateData::BindRenderTarget(const NGARenderTargetView &renderTargetView, const NGADepthStencilView &depthStencilView)
	{
		mCommandContext.BindRenderTarget(renderTargetView, depthStencilView);
	}

	void StateData::MapBufferData(const NGABuffer &buffer, void *data)
	{
		mCommandContext.MapBufferData(buffer, data);
	}

	void StateData::DrawIndexed(const IndexBuffer &buffer)
	{
		mCommandContext.DrawIndexed((unsigned int)buffer.GetNumIndices());
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