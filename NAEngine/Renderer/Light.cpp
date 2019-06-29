#include "Light.h"

#include <algorithm>
#include <vector>

#include "Base/Memory/Memory.h"

namespace na
{
	std::vector<Light*> Lights;

	Light* CreateLight(LightType type)
	{
		Light *pLight = (Light*)NA_ALLOC_ALIGNED(sizeof(Light), 16);
		Lights.push_back(pLight);
		
		pLight->mType = (int32_t)type;
		pLight->mEnabled = true;

		return pLight;
	}

	void DestroyLight(Light *pLight)
	{
		Lights.erase(std::remove(Lights.begin(), Lights.end(), pLight), Lights.end());
		NA_FREE_ALIGNED(pLight);
	}
}