#include "MaterialAsset.h"

#include "Base/Streaming/Stream.h"
#include "Base/File/File.h"
#include "Base/Util/Serialize.h"
#include "Renderer/Material/StaticMaterial.h"
#include "Renderer/Material/DynamicMaterial.h"
#include "Renderer/Resources/Texture.h"

#include "Renderer/Scene/ForwardRenderer.h"

namespace na
{
	static bool OnMaterialLoad(const AssetID &id, const std::string &filename, const AssetFileHeader &header);
	static void OnMaterialUnload(const AssetID &id);

	static bool OnShaderLoad(const AssetID &id, const std::string &filename, const AssetFileHeader &header);
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
		shaderType.mMinVersion = 2;
		shaderType.mMaxVersion = 2;
		RegisterAssetType(shaderType);

		return true;
	}

	void MaterialSystemShutdown()
	{
		NA_ASSERT(StaticMaterial::ReportEmpty(), "There were still materials allocated during shutdown!");
		StaticMaterial::ReleaseAll();

		NA_ASSERT(Shader::ReportEmpty(), "There were still shaders allocated during shutdown!");
		Shader::ReleaseAll();
	}

	Material* GetMaterialByAssetID(AssetID id)
	{
		if (StaticMaterial::Exists(id)) {
			return StaticMaterial::Get(id);
		}

		return nullptr;
	}



	static bool OnMaterialLoad(const AssetID &id, const std::string &filename, const AssetFileHeader &header)
	{
		DeserializationParameterMap params = ParseFile(filename);

		const AssetID shaderID = RequestAsset(params["shaderFile"].AsFilepath());
		Shader* shader = Shader::Get(shaderID);

		void *parameterData = nullptr;
		size_t calculatedSize = 0;

		std::vector<const Texture*> textures;
		
		if (params.HasChild("parameters")) {
			constexpr size_t MAX_MATERIAL_PARAMETER_BYTE_LENGTH = 128;
			parameterData = NA_ALLOC(MAX_MATERIAL_PARAMETER_BYTE_LENGTH);

			for (auto &parameter : params["parameters"].childrenArray) {
				std::string type = parameter.meta["type"];

				if (type == "texture") {
					if (parameter.AsString() == ":depth") {
						textures.push_back(&ForwardRenderer::mShadowMap.GetDepthMap());
					}
					else {
						AssetID texID = RequestAsset(parameter.AsFilepath());
						textures.push_back(Texture::Get(texID));
					}
				} else {
					parameter.AsHLSLType((unsigned char*)parameterData + calculatedSize, type);
				}

				calculatedSize += GetFormatByteSize(GetFormatFromString(type.c_str()));
			}
		}

		bool success = false;
		if (params["dynamic"].AsBool(false)) {
			DynamicMaterial *pMat = DynamicMaterial::Create(id);
			NA_ASSERT_RETURN_VALUE(pMat != nullptr, false, "Failed to allocate dynamic material.");
			success = pMat->Initialize(shader, calculatedSize, params["parameters"], parameterData, textures);
		} else {
			StaticMaterial *pMat = StaticMaterial::Create(id);
			NA_ASSERT_RETURN_VALUE(pMat != nullptr, false, "Failed to allocate static material.");
			success = pMat->Initialize(shader, parameterData, calculatedSize, textures);
		}

		if (parameterData != nullptr) {
			NA_FREE(parameterData);
		}

		return success;
	}

	static void OnMaterialUnload(const AssetID &id)
	{
		if (StaticMaterial::Exists(id)) {
			// Release assets here
			StaticMaterial* material = StaticMaterial::Get(id);
			ReleaseAsset(material->GetShader()->GetID());
			for (auto& tex : material->GetTextures()) {
				ReleaseAsset(tex->GetID());
			}

			return StaticMaterial::Release(id);
		} else if (DynamicMaterial::Exists(id)) {
			DynamicMaterial* material = DynamicMaterial::Get(id);
			ReleaseAsset(material->GetShader()->GetID());
			for (auto& tex : material->GetTextures()) {
				ReleaseAsset(tex->GetID());
			}

			return DynamicMaterial::Release(id);
		} else {
			NA_ASSERT(false, "Unknown material type for material %s", GetAssetFilename(id));
		}
	}




	static bool OnShaderLoad(const AssetID &id, const std::string &filename, const AssetFileHeader &header)
	{
		Shader *pShader = Shader::Create(id);
		NA_ASSERT_RETURN_VALUE(pShader != nullptr, false, "Failed to allocate shader.");

		DeserializationParameterMap params = ParseFile(filename);

		auto vsParams = params["vertexShader"];
		auto psParams = params["pixelShader"];

		// Vertex input
		NGAVertexFormatDesc vertexFormatDesc;
		for (auto &elem : vsParams["inputs"].childrenArray) {
			NGAVertexAttribute attr;

			attr.mSemanticType = GetSemanticType(elem["semanticName"].AsString().c_str());
			attr.mSemanticIndex = elem["semanticIndex"].AsInt(0);

			std::string format = elem["type"].AsString();
			attr.mFormat = GetFormatFromString(format.c_str());

			vertexFormatDesc.mAttributes.push_back(attr);
		}

		return pShader->Initialize(params["file"].AsFilepath(), vertexFormatDesc);
	}

	static void OnShaderUnload(const AssetID &id)
	{
		Shader::Release(id);
	}
}