#include "MaterialAsset.h"

#include "Base/Streaming/Stream.h"
#include "Base/File/File.h"
#include "Base/Util/Serialize.h"
#include "Renderer/Material/StaticMaterial.h"

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
		StaticMaterial *pMat = StaticMaterial::Create(id);
		NA_ASSERT_RETURN_VALUE(pMat != nullptr, false, "Failed to allocate material.");

		DeserializationParameterMap params = ParseFile(filename);

		const AssetID shaderID = RequestAsset(params["shaderFile"].AsFilepath());

		void *parameterData = nullptr;
		size_t calculatedSize = 0;

		if (params.Exists("parameters")) {
			constexpr size_t MAX_MATERIAL_PARAMETER_BYTE_LENGTH = 128;
			parameterData = NA_ALLOC(MAX_MATERIAL_PARAMETER_BYTE_LENGTH);

			for (auto &parameter : params["parameters"].childrenArray) {
				std::string type = parameter.meta["type"];
				parameter.AsHLSLType((unsigned char*)parameterData + calculatedSize, type);

				calculatedSize += GetFormatByteSize(GetFormatFromString(type));
			}
		}

		const bool success = pMat->Initialize(shaderID, parameterData, calculatedSize);

		if (parameterData != nullptr) {
			NA_FREE(parameterData);
		}

		return success;
	}

	static void OnMaterialUnload(const AssetID &id)
	{
		StaticMaterial::Release(id);
	}




	static bool OnShaderLoad(const AssetID &id, const std::string &filename)
	{
		Shader *pShader = Shader::Create(id);
		NA_ASSERT_RETURN_VALUE(pShader != nullptr, false, "Failed to allocate shader.");

		DeserializationParameterMap params = ParseFile(filename);

		// VERTEX SHADER
		VertexShader vs;
		auto vsParams = params["vertexShader"];
		vs.Initialize(vsParams["file"].AsFilepath());

		std::vector<InputElement> inputElements;
		for (auto &elem : vsParams["inputs"].childrenArray) {
			InputElement ie;

			ie.mSemantic = elem["semanticName"].AsString();
			ie.mIndex = elem["index"].AsInt(0);

			std::string format = elem["type"].AsString();
			ie.mFormat = GetFormatFromString(format);

			inputElements.push_back(ie);
		}

		InputLayout inputLayout;
		inputLayout.Initialize(inputElements, vs);

		// PIXEL SHADER
		PixelShader ps;
		auto psParams = params["pixelShader"];
		ps.Initialize(psParams["file"].AsFilepath());

		// SHADER
		return pShader->Initialize(vs, ps, inputLayout);
	}

	static void OnShaderUnload(const AssetID &id)
	{
		Shader::Release(id);
	}
}