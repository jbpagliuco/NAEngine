#pragma once

#include "Base/Streaming/Stream.h"

namespace na
{
	bool LoadShaderx(AssetID id, const char *filename, bool async);

	bool LoadMatx(AssetID id, const char *filename, bool async);
}