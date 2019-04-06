#pragma once

#include "GameComponent.h"

#include "Base/Debug/Log.h"

namespace na
{
	class TestComponent : public GameComponent
	{
	public:
		inline void Update() override { LogInfo("TEST", "Test.ing"); }
	};
}