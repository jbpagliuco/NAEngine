#pragma once

#include "PxPhysicsAPI.h"

namespace na
{
	class PhysicsErrorHandler : public physx::PxErrorCallback
	{
		virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;
	};
}