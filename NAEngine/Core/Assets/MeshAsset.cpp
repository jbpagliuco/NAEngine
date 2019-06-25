#include "MeshAsset.h"

#include <sstream>
#include <vector>

#include <DirectXMath.h>

#include "Base/Streaming/Stream.h"
#include "Base/File/File.h"
#include "Base/Util/String.h"
#include "Base/Util/Util.h"
#include "Renderer/Mesh.h"

namespace na
{
	static bool OnMeshOBJLoad(const AssetID &id, const std::string &filename);
	static void OnMeshUnload(const AssetID &id);

	static bool CreateMeshFromOBJ(Mesh *mesh, const std::string &filename);

	bool MeshSystemInit()
	{
		AssetType meshType;
		meshType.mExt = "objx";
		meshType.mOnLoad = OnMeshOBJLoad;
		meshType.mOnUnload = OnMeshUnload;
		RegisterAssetType(meshType);

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
	
	static void OnMeshUnload(const AssetID &id)
	{
		Mesh::Release(id);
	}



	static bool CreateMeshFromOBJ(Mesh *mesh, const std::string &filename)
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

		MeshData meshData;
		meshData.vertices = &(vertices[0]);
		meshData.numVertices = vertices.size();
		meshData.vertexStride = sizeof(Vertex);
		meshData.indices = &(indices[0]);
		meshData.numIndices = indices.size();

		mesh->Initialize(meshData);

		return true;
	}
}