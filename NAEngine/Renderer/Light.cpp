#include "Light.h"

#include <vector>

namespace na
{
	std::vector<Light> Lights;

	Light* CreateLight(LightType type)
	{
		Lights.push_back(Light());
		
		Light *pLight = &Lights.back();
		pLight->mType = (int32_t)type;
		pLight->mEnabled = true;

		return pLight;
	}
}