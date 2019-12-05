#include "MaterialAsset.h"

#include "Base/Streaming/Stream.h"
#include "Base/File/File.h"
#include "Base/Util/Serialize.h"
#include "Renderer/Material/StaticMaterial.h"
#include "Renderer/Material/DynamicMaterial.h"

namespace na
{
	static bool OnMaterialLoad(const AssetID &id, const std::string &filename);
	static void OnMaterialUnload(const AssetID &id);

	static bool OnShaderLoad(const AssetID &id, const std::string &filename);
	static void OnShaderUnload(const AssetID &id);


	bool MaterialSystemInit()
	{
		AssetType matType;
		matType.mExt = "matx";
		matType.mOnLoad = OnMaterialLoad;
		matType.mOnUnload = OnMaterialUnload;
		RegisterAssetType(matType);

		AssetType shaderType;
		shaderType.mExt = "shaderx";
		shaderType.mOnLoad = OnShaderLoad;
		shaderType.mOnUnload = OnShaderUnload;
		RegisterAssetType(shaderType);

		return true;
	}

	void MaterialSystemShutdown()
	{
		NA_ASSERT(StaticMaterial::NumInstances() == 0, "There were still materials allocated during shutdown!");
		StaticMaterial::ReleaseAll();

		NA_ASSERT(Shader::NumInstances() == 0, "There were still shaders allocated during shutdown!");
		Shader::ReleaseAll();
	}

	Material* GetMaterialByAssetID(AssetID id)
	{
		if (StaticMaterial::Exists(id)) {
			return StaticMaterial::Get(id);
		}

		return nullptr;
	}



	static bool OnMaterialLoad(const AssetID &id, const std::string &filename)
	{
		DeserializationParameterMap params = ParseFile(filename);

		const AssetID shaderID = RequestAsset(params["shaderFile"].AsFilepath());

		void *parameterData = nullptr;
		size_t calculatedSize = 0;

		std::vector<AssetID> textures;
		
		if (params.HasChild("parameters")) {
			constexpr size_t MAX_MATERIAL_PARAMETER_BYTE_LENGTH = 128;
			parameterData = NA_ALLOC(MAX_MATERIAL_PARAMETER_BYTE_LENGTH);

			for (auto &parameter : params["parameters"].childrenArray) {
				std::string type = parameter.meta["type"];

				if (type == "texture") {
					AssetID texID = RequestAsset(parameter.AsFilepath());
					textures.push_back(texID);
				} else {
					parameter.AsHLSLType((unsigned char*)parameterData + calculatedSize, type);
				}

				calculatedSize += GetFormatByteSize(GetFormatFromString(type));
			}
		}

		bool success = false;
		if (params["dynamic"].AsBool(false)) {
			DynamicMaterial *pMat = DynamicMaterial::Create(id);
			NA_ASSERT_RETURN_VALUE(pMat != nullptr, false, "Failed to allocate dynamic material.");
			success = pMat->Initialize(shaderID, calculatedSize, params["parameters"], parameterData, textures);
		} else {
			StaticMaterial *pMat = StaticMaterial::Create(id);
			NA_ASSERT_RETURN_VALUE(pMat != nullptr, false, "Failed to allocate static material.");
			success = pMat->Initialize(shaderID, parameterData, calculatedSize, textures);
		}

		if (parameterData != nullptr) {
			NA_FREE(parameterData);
		}

		return success;
	}

	static void OnMaterialUnload(const AssetID &id)
	{
		if (StaticMaterial::Exists(id)) {
			return StaticMaterial::Release(id);
		} else if (DynamicMaterial::Exists(id)) {
			return DynamicMaterial::Release(id);
		} else {
			NA_ASSERT(false, "Unknown material type for material %s", GetAssetFilename(id));
		}
	}




	static bool OnShaderLoad(const AssetID &id, const std::string &filename)
	{
		Shader *pShader = Shader::Create(id);
		NA_ASSERT_RETURN_VALUE(pShader != nullptr, false, "Failed to allocate shader.");

		DeserializationParameterMap params = ParseFile(filename);

		// VERTEX SHADER
		auto vsParams = params["vertexShader"];

		VertexFormatDesc vertexFormatDesc;
		for (auto &elem : vsParams["inputs"].childrenArray) {
			VertexAttribute attr;

			attr.mSemanticType = GetSemanticType(elem["semanticName"].AsString());
			attr.mSemanticIndex = elem["semanticIndex"].AsInt(0);

			std::string format = elem["type"].AsString();
			attr.mFormat = GetFormatFromString(format);

			vertexFormatDesc.mAttributes.push_back(attr);
		}

		VertexShader vs;
		vs.Initialize(vsParams["file"].AsFilepath(), vertexFormatDesc);

		// PIXEL SHADER
		PixelShader ps;
		auto psParams = params["pixelShader"];
		ps.Initialize(psParams["file"].AsFilepath());

		// SHADER
		return pShader->Initialize(vs, ps);
	}

	static void OnShaderUnload(const AssetID &id)
	{
		Shader::Release(id);
	}
}