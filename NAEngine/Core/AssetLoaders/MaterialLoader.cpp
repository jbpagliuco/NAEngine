#include "MaterialLoader.h"

#include "Base/Util/Serialize.h"

#include "Renderer/Shader.h"
#include "Renderer/Material.h"

namespace na
{
	bool LoadShaderx(AssetID id, const char *filename, bool async)
	{
		if (Shader::Exists(id)) {
			return true;
		}

		DeserializationParameterMap params = ParseFile(filename);

		// VERTEX SHADER
		auto vsParams = params["vertexShader"];
		const char *vsFilename = vsParams["file"].AsString();
		AssetID vsID = GetAssetID(vsFilename);

		VertexShader *vs = VertexShader::Create(vsID);
		vs->Initialize(vsFilename);

		std::vector<InputElement> inputElements;
		for (auto &elem : vsParams["inputs"].childrenArray) {
			InputElement ie;

			ie.mSemantic = elem["semanticName"].AsString();
			ie.mIndex = elem["index"].AsInt(0);

			const char *format = elem["type"].AsString();
			ie.mFormat = GetFormatFromString(format);

			inputElements.push_back(ie);
		}
		
		InputLayout *inputLayout = InputLayout::Create(vsID);
		inputLayout->Initialize(inputElements, *vs);

		// PIXEL SHADER
		auto psParams = params["pixelShader"];
		const char *psFilename = psParams["file"].AsString();
		AssetID psID = GetAssetID(psFilename);

		PixelShader *ps = PixelShader::Create(psID);
		ps->Initialize(psFilename);

		// SHADER
		Shader *shader = Shader::Create(id);
		shader->Initialize(vsID, psID);

		return true;
	}


	bool LoadMatx(AssetID id, const char *filename, bool async)
	{
		if (Material::Exists(id)) {
			return true;
		}

		DeserializationParameterMap params = ParseFile(filename);

		const char *shaderFilename = params["shaderFile"].AsString();
		AssetID shaderID = StreamAsset(shaderFilename);

		Material *mat = Material::Create(id);
		mat->Initialize(shaderID);

		return true;
	}
}