#pragma once

#include <DirectXMath.h>

#include "Light.h"
#include "Shader/EngineShaders/shader_common.hlsli"

struct ID3D11Buffer;
struct ID3D11RasterizerState;

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
		
		void SetViewProjMatrices(const DirectX::XMMATRIX &view, const DirectX::XMMATRIX &proj);
		void SetObjectTransform(const DirectX::XMMATRIX &transform);

		void SetLightsData(const LightsData &lights);

		void SetRasterizerState();

		int GetUserVSConstantBufferIndex()const;
		int GetUserPSConstantBufferIndex()const;

	private:
		ID3D11Buffer *mViewProjBuffer;
		ID3D11Buffer *mObjectDataBuffer;
		ID3D11Buffer *mLightsBuffer;

		ID3D11RasterizerState *mRasterizerState;
	};
}