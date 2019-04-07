#include "InputLayout.h"

#include <d3d11.h>

#include "RendererD3D.h"
#include "ShaderProgram.h"

namespace na
{
	NA_FACTORY_BUILD(InputLayout);

	static DXGI_FORMAT VertexAttributeFormats[] = {
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_R32G32_FLOAT,
		DXGI_FORMAT_R32G32B32_FLOAT,
		DXGI_FORMAT_R32G32B32A32_FLOAT
	};

	bool InputLayout::Initialize(const std::vector<InputElement> &elems, const VertexShader &vertexShader)
	{
#if defined(NA_D3D11)
		std::vector<D3D11_INPUT_ELEMENT_DESC> elementDescs;
		for (auto &elem : elems) {
			D3D11_INPUT_ELEMENT_DESC element;
			element.InputSlot = 0;
			element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			element.InstanceDataStepRate = 0;
			element.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

			element.SemanticName = elem.mSemantic;
			element.SemanticIndex = elem.mIndex;
			element.Format = (DXGI_FORMAT)elem.mFormat;

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