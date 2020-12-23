#pragma once

#include "Renderer/Scene/Renderables/BillboardInstance.h"

#include "GameComponent.h"

namespace na
{
	class BillboardComponent : public GameComponentBase<GameComponentType::BILLBOARD>
	{
	public:
		virtual void Deserialize(DeserializationParameterMap &params) override;

		virtual void Activate() override;
		virtual void Deactivate() override;

		virtual void UpdateLate(float deltaTime) override;

		bool GetVisible()const;
		void SetVisible(bool visible);

	private:
		AssetID mTextureID;

		BillboardInstance mBillboardInstance;
	};
}