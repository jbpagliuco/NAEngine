#pragma once

#include "Base/Streaming/Stream.h"

namespace na
{
	bool LoadShaderx(AssetID id, const std::string &filename, bool async);

	bool LoadMatx(AssetID id, const std::string &filename, bool async);
}