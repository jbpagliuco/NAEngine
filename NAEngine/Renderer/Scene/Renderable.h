#pragma once

#include "SceneEntity.h"

namespace na
{
	class Renderable : public SceneEntity
	{
	public:
		virtual void Render() = 0;
	};
}