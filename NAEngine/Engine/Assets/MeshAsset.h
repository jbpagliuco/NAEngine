#pragma once

#define MESH_ASSET_VERSION 4
#define MESH_ASSET_MIN_VERSION 4
#define MESH_ASSET_MAX_VERSION MESH_ASSET_VERSION

#include "Renderer/Mesh.h"

namespace na
{
	bool MeshSystemInit();
	void MeshSystemShutdown();

#if defined(_NA_TOOLS)
	bool ConvertMeshObjToMeshx(const std::string &objFilename, bool calculateTangentSpaceBasicVectors);
#endif
}