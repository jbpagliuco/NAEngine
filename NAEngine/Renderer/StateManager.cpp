#include "StateManager.h"

#include <d3d11.h>

#include "Resources/IndexBuffer.h"
#include "Resources/RenderTarget.h"
#include "Resources/VertexBuffer.h"
#include "Shader/ShaderProgram.h"

#include "Rect.h"
#include "Renderer.h"

#include "Light.h"

namespace na
{
	StateManager RendererStateManager;


	struct PerFrameData
	{
		Matrix cameraViewProj;
		Matrix lightViewProj;
	};

	struct PerObjectData
	{
		Matrix world;
		Matrix worldInverseTranspose;
	};
	


	StateManager::StateManager() :
		mBoundRenderTarget(&NGARenderTargetView::INVALID),
		mBoundDepthStencilView(&NGADepthStencilView::INVALID)
	{
	}

	bool StateManager::Initialize()
	{
		bool success = mPerFrameBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(PerFrameData));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize per frame buffer.");

		success = mObjectDataBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(PerObjectData));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize object data buffer.");

		success = mLightsBuffer.Initialize(ConstantBufferUsage::CPU_WRITE, nullptr, sizeof(LightsData));
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to initialize lights buffer.");

		return true;
	}

	void StateManager::Shutdown()
	{
		mPerFrameBuffer.Shutdown();
		mObjectDataBuffer.Shutdown();
		mLightsBuffer.Shutdown();
	}
	
	void StateManager::SetPerFrameData(const Matrix &cameraViewProj, const Matrix &lightViewProj)
	{
		PerFrameData data;
		data.cameraViewProj = cameraViewProj;
		data.lightViewProj = lightViewProj;

		mPerFrameBuffer.Map(&data);

		mCommandContext.BindConstantBuffer(mPerFrameBuffer.GetBuffer(), NGA_SHADER_STAGE_VERTEX, (int)ShaderConstantBuffers::PERFRAME);
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


	void StateManager::BindUserShaderResource(const Texture &texture, NGAShaderStage stage, int slot)
	{
		BindShaderResource(texture.GetShaderResourceView(), stage, slot + (int)TextureRegisters::USER);
	}

	void StateManager::BindUserShaderResource(const NGAShaderResourceView &view, NGAShaderStage stage, int slot)
	{
		BindShaderResource(view, stage, slot + (int)TextureRegisters::USER);
	}

	void StateManager::BindShaderResource(const Texture &texture, NGAShaderStage stage, int slot)
	{
		// Can't bind a shader resource if it's already been bound as an output
		if (texture.GetRenderTargetView() == *mBoundRenderTarget) {
			mCommandContext.BindShaderResource(NGAShaderResourceView::INVALID, stage, slot);
			return;
		}

		if (texture.GetDepthStencilView() == *mBoundDepthStencilView) {
			mCommandContext.BindShaderResource(NGAShaderResourceView::INVALID, stage, slot);
			return;
		}

		mCommandContext.BindShaderResource(texture.GetShaderResourceView(), stage, slot);
	}

	void StateManager::BindShaderResource(const NGAShaderResourceView &view, NGAShaderStage stage, int slot)
	{
		mCommandContext.BindShaderResource(view, stage, slot);
	}


	void StateManager::BindConstantBuffer(const NGABuffer &constantBuffer, NGAShaderStage stage, int slot)
	{
		BindConstantBufferRealSlot(constantBuffer, stage, slot + (int)ShaderConstantBuffers::USER);
	}

	void StateManager::BindConstantBufferRealSlot(const NGABuffer &constantBuffer, NGAShaderStage stage, int slot)
	{
		NA_ASSERT_RETURN(stage != NGA_SHADER_STAGE_ALL, "Need to implement this.");

		if (stage & NGA_SHADER_STAGE_VERTEX) {
			mCommandContext.BindConstantBuffer(constantBuffer, NGA_SHADER_STAGE_VERTEX, slot);
		}

		if (stage & NGA_SHADER_STAGE_PIXEL) {
			mCommandContext.BindConstantBuffer(constantBuffer, NGA_SHADER_STAGE_PIXEL, slot);
		}
	}


	void StateManager::BindUserSamplerState(const NGASamplerState &samplerState, NGAShaderStage stage, int slot)
	{
		BindSamplerState(samplerState, stage, slot + (int)TextureRegisters::USER);
	}

	void StateManager::BindSamplerState(const NGASamplerState &samplerState, NGAShaderStage stage, int slot)
	{
		mCommandContext.BindSamplerState(samplerState, stage, slot);
	}


	void StateManager::ClearRenderTarget(const RenderTarget &renderTarget, const float *clearColor, bool clearDepth)
	{
		mCommandContext.ClearRenderTarget(renderTarget.GetColorMap().GetRenderTargetView(), clearColor);

		if (clearDepth) {
			mCommandContext.ClearDepthStencilView(renderTarget.GetDepthMap().GetDepthStencilView());
		}
	}

	void StateManager::ClearRenderTarget(const NGARenderTargetView &renderTargetView, const float *clearColor)
	{
		mCommandContext.ClearRenderTarget(renderTargetView, clearColor);
	}

	void StateManager::ClearDepthStencilView(const NGADepthStencilView &depthStencilView)
	{
		mCommandContext.ClearDepthStencilView(depthStencilView);
	}

	void StateManager::BindRenderTarget(const RenderTarget &renderTarget)
	{
		mBoundRenderTarget = &renderTarget.GetColorMap().GetRenderTargetView();
		mBoundDepthStencilView = &renderTarget.GetDepthMap().GetDepthStencilView();
		mCommandContext.BindRenderTarget(renderTarget.GetColorMap().GetRenderTargetView(), renderTarget.GetDepthMap().GetDepthStencilView());
	}

	void StateManager::BindRenderTarget(const NGARenderTargetView &renderTargetView, const NGADepthStencilView &depthStencilView)
	{
		mBoundRenderTarget = &renderTargetView;
		mBoundDepthStencilView = &depthStencilView;
		mCommandContext.BindRenderTarget(renderTargetView, depthStencilView);
	}

	void StateManager::MapBufferData(const NGABuffer &buffer, const void *data)
	{
		mCommandContext.MapBufferData(buffer, data);
	}

	void StateManager::DrawIndexed(const IndexBuffer &buffer)
	{
		mCommandContext.DrawIndexed((unsigned int)buffer.GetNumIndices());
	}
}