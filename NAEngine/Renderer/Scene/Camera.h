#pragma once

#include "Base/Math/Transform.h"
#include "Renderer/RenderDefs.h"

namespace na
{
	class Texture;

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

	public:
		Transform mTransform;
		bool mEnabled;

		float mFOV;
		float mNear;
		float mFar;

		Texture *mRenderTarget;

		bool operator==(const Camera& other) { return other.mId == mId; }

	private:
		static int NumCameras;
		int mId;
	};
}