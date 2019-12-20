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

	class StateData
	{
	public:
		bool Initialize();
		void Shutdown();
		
		void SetViewport(const Rect &rect);
		
		void SetViewProjMatrices(const Matrix &view, const Matrix &proj);
		void SetObjectTransform(const Matrix &transform);

		void SetLightsData(const LightsData &lights);

		void SetRasterizerState();

		void BindIndexBuffer(const IndexBuffer &ib);
		void BindVertexBuffer(const VertexBuffer &vb);

		void BindInputLayout(const NGAInputLayout &inputLayout);

		void BindShader(const ShaderProgram &shader);

		void BindConstantBuffer(const NGABuffer &constantBuffer, NGAShaderStage stage, int slot);
		void BindShaderResource(const NGAShaderResourceView &view, NGAShaderStage stage, int slot);
		void BindSamplerState(const NGASamplerState &samplerState, NGAShaderStage stage, int slot);

		void MapBufferData(const NGABuffer &buffer, void *data);

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