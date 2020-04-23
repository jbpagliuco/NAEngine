#include "StateManager.h"

#include <d3d11.h>

#include "Resources/IndexBuffer.h"
#include "Resources/VertexBuffer.h"
#include "Resources/Texture.h"
#include "Shader/ShaderProgram.h"

#include "Rect.h"
#include "Renderer.h"

#include "Light.h"

namespace na
{
	StateManager RendererStateManager;

	enum class ShaderConstantBuffers
	{
		VIEWPROJ = 0,
		OBJECTDATA,
		LIGHTS,
		USER
	};

	struct PerObjectData
	{
		Matrix world;
		Matrix worldInverseTranspose;
	};
	



	bool StateManager::Initialize()
	{
		bool success = mViewProjBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(Matrix));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize view proj buffer.");

		success = mObjectDataBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(PerObjectData));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize object data buffer.");

		success = mLightsBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(LightsData));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize lights buffer.");

		return true;
	}

	void StateManager::Shutdown()
	{
		mViewProjBuffer.Shutdown();
		mObjectDataBuffer.Shutdown();
		mLightsBuffer.Shutdown();
	}
	
	void StateManager::SetViewProjMatrices(const Matrix &view, const Matrix &proj)
	{
		Matrix viewProj = proj * view;
		mViewProjBuffer.Map(&viewProj);

		mCommandContext.BindConstantBuffer(mViewProjBuffer.GetBuffer(), NGA_SHADER_STAGE_VERTEX, (int)ShaderConstantBuffers::VIEWPROJ);
	}

	void StateManager::SetObjectTransform(const Matrix &transform)
	{
		PerObjectData data;
		data.world = transform;
		data.worldInverseTranspose = transform.Inverted().Transposed();

		mObjectDataBuffer.Map(&data);

		mCommandContext.BindConstantBuffer(mObjectDataBuffer.GetBuffer(), NGA_SHADER_STAGE_VERTEX, (int)ShaderConstantBuffers::OBJECTDATA);
	}

	void StateManager::SetLightsData(const LightsData &lights)
	{
		mLightsBuffer.Map((void*)&lights);

		mCommandContext.BindConstantBuffer(mLightsBuffer.GetBuffer(), NGA_SHADER_STAGE_PIXEL, (int)ShaderConstantBuffers::LIGHTS);
	}

	void StateManager::SetViewport(const NGARect &rect)
	{
		mCommandContext.SetViewport(rect);
	}

	void StateManager::SetPrimitiveTopology(NGAPrimitiveTopology primTopology)
	{
		mCommandContext.SetPrimitiveTopology(primTopology);
	}

	void StateManager::BindIndexBuffer(const IndexBuffer &ib)
	{
		mCommandContext.BindIndexBuffer(ib.GetBuffer(), NGAIndexBufferType::IBT_32BIT);
	}

	void StateManager::BindVertexBuffer(const VertexBuffer &vb)
	{
		mCommandContext.BindVertexBuffer(vb.GetBuffer(), vb.GetVertexStride());
	}

	void StateManager::BindInputLayout(const NGAInputLayout &inputLayout)
	{
		mCommandContext.BindInputLayout(inputLayout);
	}

	void StateManager::BindShader(const ShaderProgram &shader)
	{
		mCommandContext.BindShader(shader.GetShader());
	}

	void StateManager::BindShaderResource(const NGAShaderResourceView &view, NGAShaderStage stage, int slot)
	{
		mCommandContext.BindShaderResource(view, stage, slot);
	}

	void StateManager::BindConstantBuffer(const NGABuffer &constantBuffer, NGAShaderStage stage, int slot)
	{
		NA_ASSERT_RETURN(stage != NGA_SHADER_STAGE_ALL, "Need to implement this.");

		if (stage & NGA_SHADER_STAGE_VERTEX) {
			mCommandContext.BindConstantBuffer(constantBuffer, NGA_SHADER_STAGE_VERTEX, slot + (int)ShaderConstantBuffers::USER);
		}

		if (stage & NGA_SHADER_STAGE_PIXEL) {
			mCommandContext.BindConstantBuffer(constantBuffer, NGA_SHADER_STAGE_PIXEL, slot + (int)ShaderConstantBuffers::USER);
		}
	}

	void StateManager::BindSamplerState(const NGASamplerState &samplerState, NGAShaderStage stage, int slot)
	{
		mCommandContext.BindSamplerState(samplerState, stage, slot);
	}

	void StateManager::ClearRenderTarget(const NGARenderTargetView &renderTargetView, const float *clearColor)
	{
		mCommandContext.ClearRenderTarget(renderTargetView, clearColor);
	}

	void StateManager::ClearDepthStencilView(const NGADepthStencilView &depthStencilView)
	{
		mCommandContext.ClearDepthStencilView(depthStencilView);
	}

	void StateManager::BindRenderTarget(const Texture &renderTarget)
	{
		mCommandContext.BindRenderTarget(renderTarget.GetRenderTargetView(), renderTarget.GetDepthStencilView());
	}

	void StateManager::BindRenderTarget(const NGARenderTargetView &renderTargetView, const NGADepthStencilView &depthStencilView)
	{
		mCommandContext.BindRenderTarget(renderTargetView, depthStencilView);
	}

	void StateManager::MapBufferData(const NGABuffer &buffer, void *data)
	{
		mCommandContext.MapBufferData(buffer, data);
	}

	void StateManager::DrawIndexed(const IndexBuffer &buffer)
	{
		mCommandContext.DrawIndexed((unsigned int)buffer.GetNumIndices());
	}
}