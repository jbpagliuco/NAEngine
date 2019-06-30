#include "InputLayout.h"

#include <d3d11.h>

#include "RendererD3D.h"
#include "Shader/ShaderProgram.h"

namespace na
{
	bool InputLayout::Initialize(const VertexFormatDesc &vertexFormatDesc, const VertexShader &vertexShader)
	{
#if defined(NA_D3D11)
		std::vector<D3D11_INPUT_ELEMENT_DESC> elementDescs;
		for (auto &attr : vertexFormatDesc.mAttributes) {
			D3D11_INPUT_ELEMENT_DESC element;
			element.InputSlot = 0;
			element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			element.InstanceDataStepRate = 0;

			element.Format = GetDXGIFormat(attr.mFormat);
			element.SemanticName = GetSemanticName(attr.mSemanticType);
			element.SemanticIndex = attr.mSemanticIndex;
			element.AlignedByteOffset = attr.mOffset;

			elementDescs.push_back(element);
		}

		elementDescs[0].AlignedByteOffset = 0;

		HRESULT hr = NA_RDevice->CreateInputLayout(&(elementDescs[0]), (UINT)elementDescs.size(), vertexShader.GetBuffer(), vertexShader.GetBufferSize(), &mInputLayout);
		if (FAILED(hr)) {
			return false;
		}
#endif

		return true;
	}

	void InputLayout::Shutdown()
	{
		NA_SAFE_RELEASE(mInputLayout);
	}

	void InputLayout::Bind()
	{
		NA_RContext->IASetInputLayout(mInputLayout);
	}
}