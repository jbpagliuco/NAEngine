#include "NGA/NGAResourceViews.h"

#if defined(NGA_D3D11)

#include "NGACoreInternalDX11.h"

#include "NGA/NGAResources.h"

namespace na
{
	NGA_GPU_CLASS_IMPLEMENT(NGAShaderResourceView);

	bool NGAShaderResourceView::Construct(const NGATexture &texture)
	{
		NA_ASSERT_RETURN_VALUE(!IsConstructed(), false);
		NA_ASSERT_RETURN_VALUE(texture.IsConstructed(), false);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Texture2D.MipLevels = -1;

		const NGATextureDesc &texDesc = texture.GetDesc();
		switch (texDesc.mType) {
		case NGATextureType::TEXTURE2D:
		{
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

			ID3D11Texture2D *tex;
			texture.mResource->QueryInterface(&tex);

			D3D11_TEXTURE2D_DESC texDesc;
			tex->GetDesc(&texDesc);

			desc.Format = texDesc.Format;
			
			tex->Release();
			break;
		}

		default:
			NA_ASSERT_RETURN_VALUE(false, false, "Unimplemented texture type.");
		};
		
		const HRESULT hr = NgaDx11State.mDevice->CreateShaderResourceView(texture.mResource, &desc, &mView);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "ID3D11Device::CreateShaderResourceView() failed with HRESULT %X", hr);

		return true;
	}

	void NGAShaderResourceView::Destruct()
	{
		NGA_DX11_SAFE_RELEASE(mView);
	}

	bool NGAShaderResourceView::IsConstructed()const
	{
		return mView != nullptr;
	}
}

#endif