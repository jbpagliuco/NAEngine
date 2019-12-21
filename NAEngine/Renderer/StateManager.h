#pragma once

#include "Base/Math/Matrix.h"

#include "NGA/NGACommandContext.h"

#include "Light.h"
#include "Resources/ConstantBuffer.h"
#include "Shader/EngineShaders/shader_common.hlsli"

struct ID3D11RasterizerState;

namespace na
{
	class NGABuffer;
	class NGAInputLayout;
	class NGASamplerState;
	class NGAShaderResourceView;
	class IndexBuffer;
	class ShaderProgram;
	class VertexBuffer;

	struct Rect;

	struct LightsData
	{
		Tuple4f globalAmbient;
		Tuple3f eyePosition;
		int numLights;
		Light lights[MAX_LIGHTS];
	};

	class StateManager
	{
	public:
		bool Initialize();
		void Shutdown();
		
		void SetViewProjMatrices(const Matrix &view, const Matrix &proj);
		void SetObjectTransform(const Matrix &transform);

		void SetLightsData(const LightsData &lights);

		void SetViewport(const Rect &rect);
		void SetRasterizerState(const NGARasterizerState &state);

		void SetPrimitiveTopology(NGAPrimitiveTopology primTopology);

		void BindIndexBuffer(const IndexBuffer &ib);
		void BindVertexBuffer(const VertexBuffer &vb);

		void BindInputLayout(const NGAInputLayout &inputLayout);

		void BindShader(const ShaderProgram &shader);

		void BindConstantBuffer(const NGABuffer &constantBuffer, NGAShaderStage stage, int slot);
		void BindShaderResource(const NGAShaderResourceView &view, NGAShaderStage stage, int slot);
		void BindSamplerState(const NGASamplerState &samplerState, NGAShaderStage stage, int slot);

		void ClearRenderTarget(const NGARenderTargetView &renderTargetView, const float *clearColor);
		void ClearDepthStencilView(const NGADepthStencilView &depthStencilView);
		void BindRenderTarget(const NGARenderTargetView &renderTargetView, const NGADepthStencilView &depthStencilView);

		void MapBufferData(const NGABuffer &buffer, void *data);

		void DrawIndexed(const IndexBuffer &buffer);

		int GetUserVSConstantBufferIndex()const;
		int GetUserPSConstantBufferIndex()const;

	private:
		NGACommandContext mCommandContext;

		ConstantBuffer mViewProjBuffer;
		ConstantBuffer mObjectDataBuffer;

		ConstantBuffer mLightsBuffer;
		
		ID3D11RasterizerState *mRasterizerState;
	};
}