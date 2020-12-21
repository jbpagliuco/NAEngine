#pragma once

#define MESH_ASSET_VERSION 3
#define MESH_ASSET_MIN_VERSION 2
#define MESH_ASSET_MAX_VERSION MESH_ASSET_VERSION

// 3 - Add support for multiple index buffers.

#include "Renderer/Mesh.h"

namespace na
{
	bool MeshSystemInit();
	void MeshSystemShutdown();

#if defined(_NA_TOOLS)
	bool ConvertMeshObjToMeshx(const std::string &objFilename, bool calculateTangentSpaceBasicVectors);
#endif
}