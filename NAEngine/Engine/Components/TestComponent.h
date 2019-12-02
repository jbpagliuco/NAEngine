#pragma once

#include "GameComponent.h"

#include "Base/Debug/Log.h"
#include "Base/Util/Serialize.h"

namespace na
{
	class TestComponent : public GameComponent
	{
	public:
		inline void Deserialize(DeserializationParameterMap &params)
		{
			mTestInt = params["int"].AsInt(100);
		}

		inline void Update() override { LogInfo("TEST", "%d", mTestInt); }

	private:
		int mTestInt;
	};
}