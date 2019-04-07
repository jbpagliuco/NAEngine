#pragma once

#include <DirectXMath.h>

struct ID3D11Buffer;
struct ID3D11RasterizerState;

namespace na
{
	struct Rect;

	class StateData
	{
	public:
		bool Initialize();
		void Shutdown();
		
		void SetViewport(const Rect &rect);
		
		void SetViewProjMatrices(const DirectX::XMMATRIX &view, const DirectX::XMMATRIX &proj);
		void SetObjectTransform(const DirectX::XMMATRIX &transform);

		void SetRasterizerState();

		int GetUserVSConstantBufferIndex()const;
		int GetUserPSConstantBufferIndex()const;

	private:
		ID3D11Buffer *mViewProjBuffer;
		ID3D11Buffer *mObjectDataBuffer;

		ID3D11RasterizerState *mRasterizerState;
	};
}