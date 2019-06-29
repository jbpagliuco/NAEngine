#include "MaterialAsset.h"

#include "Base/Streaming/Stream.h"
#include "Base/File/File.h"
#include "Base/Util/Serialize.h"
#include "Renderer/Material.h"

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
		NA_ASSERT(Material::NumInstances() == 0, "There were still materials allocated during shutdown!");
		Material::ReleaseAll();

		NA_ASSERT(Shader::NumInstances() == 0, "There were still shaders allocated during shutdown!");
		Shader::ReleaseAll();
	}

	static bool OnMaterialLoad(const AssetID &id, const std::string &filename)
	{
		Material *pMat = Material::Create(id);
		NA_ASSERT_RETURN_VALUE(pMat != nullptr, false, "Failed to allocate material.");

		DeserializationParameterMap params = ParseFile(filename);

		const AssetID shaderID = RequestAsset(params["shaderFile"].AsFilepath());

		constexpr size_t MAX_PROPERTY_BYTE_LENGTH = 64;
		void *propertyData = NA_ALLOC_ALIGNED(MAX_PROPERTY_BYTE_LENGTH, 16);

		size_t calculatedSize = 0;
		for (auto &prop : params["properties"].childrenArray) {
			std::string type = prop.meta["type"];
			prop.AsType((unsigned char*)propertyData + calculatedSize, type);

			calculatedSize += GetFormatByteSize(GetFormatFromString(type));
		}

		NA_ASSERT(calculatedSize <= MAX_PROPERTY_BYTE_LENGTH);

		const bool success = pMat->Initialize(shaderID, propertyData, calculatedSize);

		NA_FREE_ALIGNED(propertyData);

		return success;
	}

	static void OnMaterialUnload(const AssetID &id)
	{
		Material::Release(id);
	}



	static bool OnShaderLoad(const AssetID &id, const std::string &filename)
	{
		Shader *pShader = Shader::Create(id);
		NA_ASSERT_RETURN_VALUE(pShader != nullptr, false, "Failed to allocate shader.");

		DeserializationParameterMap params = ParseFile(filename);

		// VERTEX SHADER
		VertexShader vs;
		auto vsParams = params["vertexShader"];
		vs.Initialize(vsParams["file"].AsFilepath(), vsParams["bufferSize"].AsInt(0));

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
		ps.Initialize(psParams["file"].AsFilepath(), psParams["bufferSize"].AsInt(0));

		// SHADER
		return pShader->Initialize(vs, ps, inputLayout);
	}

	static void OnShaderUnload(const AssetID &id)
	{
		Shader::Release(id);
	}
}