#pragma once

#include "Base/Math/Transform.h"
#include "Renderer/RenderDefs.h"

namespace na
{
	class RenderTarget;

	class Camera
	{
	public:
		Camera() : 
			mEnabled(false),
			mFOV(0.0f),
			mNear(0.0f),
			mFar(0.0f),
			mRenderTarget(nullptr),
			mId(NumCameras++) { }

		Matrix GetViewProj()const;

	public:
		Transform mTransform;
		bool mEnabled;

		float mFOV;
		float mNear;
		float mFar;

		RenderTarget *mRenderTarget;

		bool operator==(const Camera& other) { return other.mId == mId; }

	private:
		static int NumCameras;
		int mId;
	};
}