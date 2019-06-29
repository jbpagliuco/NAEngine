#include "MeshAsset.h"

#include <sstream>
#include <vector>

#include <DirectXMath.h>

#include "Base/Debug/Log.h"
#include "Base/File/File.h"
#include "Base/Streaming/Stream.h"
#include "Base/Util/String.h"
#include "Base/Util/Util.h"
#include "Renderer/Mesh.h"

namespace na
{
	static bool OnMeshOBJLoad(const AssetID &id, const std::string &filename);
	static bool OnMeshxLoad(const AssetID &id, const std::string &filename);
	static void OnMeshUnload(const AssetID &id);

	static bool CreateMeshFromOBJ(Mesh *mesh, const std::string &filename);
	static bool CreateMeshFromMeshx(Mesh *mesh, const std::string &filename);

	bool MeshSystemInit()
	{
		AssetType objMeshType;
		objMeshType.mExt = "objx";
		objMeshType.mOnLoad = OnMeshOBJLoad;
		objMeshType.mOnUnload = OnMeshUnload;
		RegisterAssetType(objMeshType);

		AssetType meshxMeshType;
		meshxMeshType.mExt = "meshx";
		meshxMeshType.mOnLoad = OnMeshxLoad;
		meshxMeshType.mOnUnload = OnMeshUnload;
		RegisterAssetType(meshxMeshType);

		return true;
	}

	void MeshSystemShutdown()
	{
		NA_ASSERT(Mesh::NumInstances() == 0, "There were still meshes allocated during shutdown!");
		Mesh::ReleaseAll();
	}

	static bool OnMeshOBJLoad(const AssetID &id, const std::string &filename)
	{
		Mesh *pMesh = Mesh::Create(id);
		NA_ASSERT_RETURN_VALUE(pMesh != nullptr, false, "Failed to allocate mesh.");

		return CreateMeshFromOBJ(pMesh, filename);
	}

	static bool OnMeshxLoad(const AssetID &id, const std::string &filename)
	{
		Mesh *pMesh = Mesh::Create(id);
		NA_ASSERT_RETURN_VALUE(pMesh != nullptr, false, "Failed to allocate mesh.");

		return CreateMeshFromMeshx(pMesh, filename);
	}
	
	static void OnMeshUnload(const AssetID &id)
	{
		Mesh::Release(id);
	}


	static bool CreateMeshFromOBJ(MeshData &meshData, const std::string &filename)
	{
		struct FaceIndexData
		{
			uint32_t pos;
			uint32_t norm;
			uint32_t tex;
		};

		File file(filename, std::ios::in);

		typedef na::Vector3<float> Position;
		typedef na::Vector3<float> Normal;
		typedef na::Vector2<float> TexCoord;

		std::vector<Position> positions;
		std::vector<Normal> normals;
		std::vector<TexCoord> texCoords;
		std::vector<FaceIndexData> faces;

		/////////////////////////////////////////////////
		std::string line;
		while (file.ReadLine(line)) {
			Trim(line);

			std::stringstream ss(line);

			// Strip the prefix
			std::string garbage;
			ss >> garbage;

			switch (line[0]) {
			case 'v':
				// Vertex data
				switch (line[1]) {
				case ' ':
				{
					Position p;
					ss >> p.x >> p.y >> p.z;
					positions.push_back(p);
					break;
				}

				case 'n':
				{
					Normal n;
					ss >> n.x >> n.y >> n.z;
					normals.push_back(n);
					break;
				}

				case 't':
				{
					TexCoord texCoord;
					ss >> texCoord.x >> texCoord.y;
					texCoords.push_back(texCoord);
					break;
				}
				}

				break;

			case 'f':
			{
				// TODO
				NA_ASSERT_RETURN_VALUE(normals.size() > 0 && texCoords.size() > 0, false, "Meshes must have both normal and tex coord data.");

				const int numVerticesInFace = 3;
				FaceIndexData indices[numVerticesInFace];

				for (int i = 0; i < numVerticesInFace; ++i) {
					std::string faceString;
					ss >> faceString;
					std::stringstream faceSS(faceString);

					std::string indexString;
					std::getline(faceSS, indexString, '/');
					indices[i].pos = std::stoul(indexString) - 1;

					std::getline(faceSS, indexString, '/');
					indices[i].tex = std::stoul(indexString) - 1;

					std::getline(faceSS, indexString, '/');
					indices[i].norm = std::stoul(indexString) - 1;

					faces.push_back(indices[i]);
				}
				break;
			}
			}
		}

		NA_ASSERT_RETURN_VALUE(positions.size() > 0, false);
		NA_ASSERT_RETURN_VALUE(faces.size() > 0, false);

		// Remove duplicates and create mesh
		std::map<std::string, uint32_t> vertexIds;
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 norm;
			DirectX::XMFLOAT2 tex;
		};
		std::vector<Vertex> vertices;
		std::vector<IndexType> indices;

		for (auto &face : faces) {
			std::string faceString = std::to_string(face.pos) + "/" + std::to_string(face.norm) + "/" + std::to_string(face.tex);

			if (vertexIds.find(faceString) == vertexIds.end()) {
				Vertex vertex;

				Position &pos = positions[face.pos];
				vertex.pos = DirectX::XMFLOAT3(pos.vArray);

				Normal &norm = normals[face.norm];
				vertex.norm = DirectX::XMFLOAT3(norm.vArray);

				TexCoord &texCoord = texCoords[face.tex];
				vertex.tex = DirectX::XMFLOAT2(texCoord.vArray);

				vertices.push_back(vertex);

				IndexType index = (IndexType)(vertices.size() - 1);
				vertexIds[faceString] = index;
				indices.push_back(index);
			} else {
				IndexType index = vertexIds[faceString];
				indices.push_back(index);
			}
		}

		
		meshData.numVertices = vertices.size();
		meshData.vertexStride = sizeof(Vertex);
		meshData.numIndices = indices.size();

		// The caller must free these.
		meshData.vertices = NA_ALLOC(meshData.numVertices * meshData.vertexStride);
		memcpy(meshData.vertices, vertices.data(), meshData.numVertices * meshData.vertexStride);

		meshData.indices = (IndexType*)NA_ALLOC(meshData.numIndices * sizeof(IndexType));
		memcpy(meshData.indices, indices.data(), meshData.numIndices * sizeof(IndexType));

		return true;
	}

	static bool CreateMeshFromOBJ(Mesh *mesh, const std::string &filename)
	{
		MeshData meshData;
		bool success = CreateMeshFromOBJ(meshData, filename);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to create mesh from file %s", filename.c_str());

		success = mesh->Initialize(meshData);
		
		NA_FREE(meshData.vertices);
		NA_FREE(meshData.indices);

		return success;
	}

	static bool CreateMeshFromMeshx(Mesh *mesh, const std::string &filename)
	{
		// File format:
		// <Version - 4 bytes>
		// <Vertex Stride - 4 bytes>
		// <Num Vertices - 8 bytes>
		// <Num Indices - 8 bytes>
		// <Vertices - Vertex Stride * Num Vertices>
		// <Indices - sizeof(IndexType) * Num Indices>

		File file(filename, std::ios::in | std::ios::binary);

		uint32_t version;
		file.Read<uint32_t>(version);
		NA_ASSERT_RETURN_VALUE(version >= MESH_ASSET_MIN_VERSION && version <= MESH_ASSET_MAX_VERSION, 
			false, 
			"Trying to load mesh with invalid version number (%d). File: %s", version, filename.c_str());

		uint32_t vertexStride;
		bool success = file.Read<uint32_t>(vertexStride);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to read vertexStride from mesh file: %s", filename.c_str());

		uint64_t numVertices;
		success = file.Read<uint64_t>(numVertices);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to read numVertices from mesh file: %s", filename.c_str());

		uint64_t numIndices;
		success = file.Read<uint64_t>(numIndices);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to read numIndices from mesh file: %s", filename.c_str());

		void *vertexData = NA_ALLOC(vertexStride * numVertices);
		success = file.ReadBytes((char*)vertexData, vertexStride * numVertices);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to read vertexData from mesh file: %s", filename.c_str());

		void *indexData = NA_ALLOC(sizeof(IndexType) * numIndices);
		success = file.ReadBytes((char*)indexData, sizeof(IndexType) * numIndices);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to read indexData from mesh file: %s", filename.c_str());

		MeshData meshData;
		meshData.vertices = vertexData;
		meshData.numVertices = numVertices;
		meshData.vertexStride = vertexStride;
		meshData.indices = (IndexType*)indexData;
		meshData.numIndices = numIndices;
		
		const bool initialized = mesh->Initialize(meshData);

		NA_FREE(vertexData);
		NA_FREE(indexData);

		return initialized;
	}


#if defined(_NA_TOOLS)
	bool ConvertMeshObjToMeshx(const std::string &filename)
	{
		MeshData meshData;
		const bool success = CreateMeshFromOBJ(meshData, filename);
		NA_ASSERT_RETURN_VALUE(success, false, "Failed to create mesh from %s", filename.c_str());

		const std::string outFilename = DropFileExt(filename) + ".meshx";
		File file(outFilename, std::ios::out | std::ios::binary);

#define WRITE(cat, x) if (!(x)) { NA_ASSERT_RETURN_VALUE(false, false, "Failed to write %s", cat); }
		WRITE("Version", file.Write<uint32_t>(MESH_ASSET_VERSION));
		WRITE("Vertex Stride", file.Write<uint32_t>((int32_t)meshData.vertexStride));
		WRITE("Num Vertices", file.Write<uint64_t>(meshData.numVertices));
		WRITE("Num Indices", file.Write<uint64_t>(meshData.numIndices));
		WRITE("Vertices", file.WriteBytes((char*)meshData.vertices, meshData.numVertices * meshData.vertexStride));
		WRITE("Indices", file.WriteBytes((char*)meshData.indices, meshData.numIndices * sizeof(IndexType)));
#undef WRITE

		NA_FREE(meshData.vertices);
		NA_FREE(meshData.indices);

		LogInfo("Mesh", "Successfully converted mesh to meshx. Output file: %s", outFilename);

		return true;
	}
#endif
}