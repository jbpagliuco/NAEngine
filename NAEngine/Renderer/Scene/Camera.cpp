#include "Camera.h"

#include "Renderer/Renderer.h"

namespace na
{
	int Camera::NumCameras = 0;

	Matrix Camera::GetView()const
	{
		return mTransform.GetMatrix().Inverted();
	}

	Matrix Camera::GetProj()const
	{
		return Matrix::PerspectiveFOVLH(mFOV, NA_Renderer->GetWindow().GetAspectRatio(), mNear, mFar);
	}

	Matrix Camera::GetViewProj()const
	{
		return GetProj() * GetView();
	}
}