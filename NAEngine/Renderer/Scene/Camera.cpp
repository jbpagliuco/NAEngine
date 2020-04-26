#include "Camera.h"

#include "Renderer/Renderer.h"

namespace na
{
	int Camera::NumCameras = 0;

	Matrix Camera::GetViewProj()const
	{
		const Matrix cameraView = mTransform.GetMatrix().Inverted();
		const Matrix cameraProj = Matrix::PerspectiveFOVLH(mFOV, NA_Renderer->GetWindow().GetAspectRatio(), mNear, mFar);

		return cameraProj * cameraView;
	}
}