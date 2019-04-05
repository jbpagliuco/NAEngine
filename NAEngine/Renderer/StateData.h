#pragma once

struct ID3D11RasterizerState;

namespace na
{
	struct Rect;

	class StateData
	{
	public:
		void Shutdown();
		
		void SetViewport(const Rect &rect);
		
		void SetRasterizerState();

	private:
		ID3D11RasterizerState *mRasterizerState = nullptr;
	};
}