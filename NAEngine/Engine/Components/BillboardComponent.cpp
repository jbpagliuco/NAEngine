#include "BillboardComponent.h"

#include "Base/Streaming/Stream.h"

#include "Renderer/Scene/Scene.h"

#include "Engine/Assets/TextureAsset.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/StaticMeshComponent.h"
#include "Engine/World/GameObject.h"

namespace na
{
	void BillboardComponent::Deserialize(DeserializationParameterMap &params)
	{
		mTextureID = RequestAsset(params["texture"].AsFilepath());

		TextureAsset* textureAsset = TextureAsset::Get(mTextureID);
		mBillboardInstance.Initialize(&(textureAsset->GetTexture()));
		mBillboardInstance.mVisible = true;

		mTriggerDistance = params["distance"].AsFloat(30.0f);
	}

	void BillboardComponent::Activate()
	{
		Scene::Get()->AddRenderable(&mBillboardInstance);
	}

	void BillboardComponent::Deactivate()
	{
		Scene::Get()->RemoveRenderable(&mBillboardInstance);

		mBillboardInstance.Shutdown();
		ReleaseAsset(mTextureID);
	}

	void BillboardComponent::UpdateLate(float deltaTime)
	{
		mBillboardInstance.SetWorldTransform(GetOwner()->mTransform.GetMatrix());

		Camera* camera = Scene::Get()->GetRenderCamera();
		const float distance = (camera->mTransform.mPosition - GetOwner()->mTransform.mPosition).V3Length();

		SetVisible(distance > mTriggerDistance);
		GetOwner()->GetComponentOfType<StaticMeshComponent>()->SetVisible(!GetVisible());
	}


	bool BillboardComponent::GetVisible()const
	{
		return mBillboardInstance.mVisible;
	}

	void BillboardComponent::SetVisible(bool visible)
	{
		mBillboardInstance.mVisible = visible;
	}
}