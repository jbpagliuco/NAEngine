#pragma once

#include "Base/Streaming/Stream.h"

namespace na
{
	bool LoadMeshOBJ(AssetID id, const std::string &filename, bool async);
}