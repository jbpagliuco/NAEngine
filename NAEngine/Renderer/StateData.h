#pragma once

#include "Base/Math/Matrix.h"

#include "ConstantBuffer.h"
#include "Light.h"
#include "Shader/EngineShaders/shader_common.hlsli"

struct ID3D11RasterizerState;

namespace DirectX {
	struct XMMATRIX;
}

namespace na
{
	struct Rect;

	struct LightsData
	{
		DirectX::XMFLOAT4 globalAmbient;
		DirectX::XMFLOAT3 eyePosition;
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

		int GetUserVSConstantBufferIndex()const;
		int GetUserPSConstantBufferIndex()const;

	private:
		ConstantBuffer mViewProjBuffer;
		ConstantBuffer mObjectDataBuffer;

		ConstantBuffer mLightsBuffer;
		
		ID3D11RasterizerState *mRasterizerState;
	};
}