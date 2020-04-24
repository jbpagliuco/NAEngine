#pragma once

#include "../SceneEntity.h"

namespace na
{
	class RenderableInstance : public SceneEntity
	{
	public:
		virtual void Render(bool bindMaterial = true) = 0;
	};
}