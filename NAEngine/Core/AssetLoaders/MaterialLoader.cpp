#include "MaterialLoader.h"

#include "Base/Util/Serialize.h"

#include "Renderer/Shader.h"
#include "Renderer/Material.h"

namespace na
{
	bool LoadShaderx(AssetID id, const std::string &filename, bool async)
	{
		if (Shader::Exists(id)) {
			return true;
		}

		DeserializationParameterMap params = ParseFile(filename);

		// VERTEX SHADER
		auto vsParams = params["vertexShader"];
		std::string vsFilename = vsParams["file"].AsFilepath();
		AssetID vsID = GetAssetID(vsFilename);

		VertexShader *vs = VertexShader::Create(vsID);
		vs->Initialize(vsFilename, vsParams["bufferSize"].AsInt(0));

		std::vector<InputElement> inputElements;
		for (auto &elem : vsParams["inputs"].childrenArray) {
			InputElement ie;

			ie.mSemantic = elem["semanticName"].AsString();
			ie.mIndex = elem["index"].AsInt(0);

			std::string format = elem["type"].AsString();
			ie.mFormat = GetFormatFromString(format);

			inputElements.push_back(ie);
		}
		
		InputLayout *inputLayout = InputLayout::Create(vsID);
		inputLayout->Initialize(inputElements, *vs);

		// PIXEL SHADER
		auto psParams = params["pixelShader"];
		std::string psFilename = psParams["file"].AsFilepath();
		AssetID psID = GetAssetID(psFilename);

		PixelShader *ps = PixelShader::Create(psID);
		ps->Initialize(psFilename, psParams["bufferSize"].AsInt(0));

		// SHADER
		Shader *shader = Shader::Create(id);
		shader->Initialize(vsID, psID);

		return true;
	}


	bool LoadMatx(AssetID id, const std::string &filename, bool async)
	{
		if (Material::Exists(id)) {
			return true;
		}

		DeserializationParameterMap params = ParseFile(filename);

		std::string shaderFilename = params["shaderFile"].AsString();
		AssetID shaderID = StreamAsset(shaderFilename);

		constexpr size_t MAX_PROPERTY_BYTE_LENGTH = 16;
		void *propertyData = NA_ALLOC_ALIGNED(MAX_PROPERTY_BYTE_LENGTH, 16);

		size_t calculatedSize = 0;
		for (auto &prop : params["properties"].childrenArray) {
			std::string type = prop.meta["type"];
			prop.AsType((unsigned char*)propertyData + calculatedSize, type);

			calculatedSize += GetFormatByteSize(GetFormatFromString(type));
		}

		NA_ASSERT(calculatedSize <= MAX_PROPERTY_BYTE_LENGTH);

		Material *mat = Material::Create(id);
		mat->Initialize(shaderID, propertyData, calculatedSize);

		NA_FREE_ALIGNED(propertyData);

		return true;
	}
}