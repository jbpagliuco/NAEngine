#include "LightComponent.h"

#include "Base/Math/Transform.h"
#include "Renderer/Light.h"
#include "Renderer/Scene/Scene.h"

namespace na
{
	void LightComponent::Deactivate()
	{
		Scene::Get()->RemoveLight(mLight);
		DestroyLight(mLight);
	}

	void LightComponent::UpdateLate(float deltaTime)
	{
		// Update some of the light properties from our transform.
		mLight->mPosition = DirectX::XMFLOAT3(mTransform->mPosition.GetX(), mTransform->mPosition.GetY(), mTransform->mPosition.GetZ());
		static const DirectX::XMFLOAT4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);
		static const DirectX::XMVECTOR zAxisVector = DirectX::XMLoadFloat4(&zAxis);
		DirectX::XMVECTOR rot = DirectX::XMVectorSet(mTransform->mRotation.x, mTransform->mRotation.y, mTransform->mRotation.z, mTransform->mRotation.w);
		const DirectX::XMVECTOR vForward = DirectX::XMVector3Rotate(zAxisVector, rot);
		DirectX::XMStoreFloat3(&mLight->mDirection, vForward);
	}

	void LightComponent::SetLight(Light *pLight)
	{
		mLight = pLight;

		Scene::Get()->AddLight(mLight);
	}




	void DirectionalLightComponent::Deserialize(DeserializationParameterMap &params)
	{
		Light *pLight = CreateLight(LightType::DIRECTIONAL);
		
		pLight->mEnabled = params["enabled"].AsBool(true);
		pLight->mColor = params["color"].AsColor();
		pLight->mIntensity = params["intensity"].AsFloat(1.0f);
		pLight->mConstantAttenuation = params["constantAttenuation"].AsFloat();
		pLight->mLinearAttenuation = params["linearAttenuation"].AsFloat();
		pLight->mQuadraticAttenuation = params["quadraticAttenuation"].AsFloat();

		SetLight(pLight);
	}


	void PointLightComponent::Deserialize(DeserializationParameterMap &params)
	{
		Light *pLight = CreateLight(LightType::POINT);

		pLight->mEnabled = params["enabled"].AsBool(true);
		pLight->mColor = params["color"].AsColor();
		pLight->mIntensity = params["intensity"].AsFloat(1.0f);
		pLight->mRadius = params["radius"].AsFloat(15.0f);
		pLight->mConstantAttenuation = params["constantAttenuation"].AsFloat();
		pLight->mLinearAttenuation = params["linearAttenuation"].AsFloat();
		pLight->mQuadraticAttenuation = params["quadraticAttenuation"].AsFloat();

		SetLight(pLight);
	}


	void SpotLightComponent::Deserialize(DeserializationParameterMap &params)
	{
		Light *pLight = CreateLight(LightType::SPOT);

		pLight->mEnabled = params["enabled"].AsBool(true);
		pLight->mColor = params["color"].AsColor();
		pLight->mIntensity = params["intensity"].AsFloat(1.0f);
		pLight->mSpotLightAngle = params["spotLightAngle"].AsRadian(45.0f);
		pLight->mConstantAttenuation = params["constantAttenuation"].AsFloat();
		pLight->mLinearAttenuation = params["linearAttenuation"].AsFloat();
		pLight->mQuadraticAttenuation = params["quadraticAttenuation"].AsFloat();

		SetLight(pLight);
	}
}