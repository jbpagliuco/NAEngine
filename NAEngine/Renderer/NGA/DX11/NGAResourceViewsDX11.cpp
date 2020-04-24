#include "NGA/NGAResourceViews.h"

#if defined(NGA_D3D11)

#include "NGACoreInternalDX11.h"

#include "NGA/NGAResources.h"
#include "NGA/NGASwapChain.h"

namespace na
{
	NGA_GPU_CLASS_IMPLEMENT(NGAShaderResourceView);
	NGA_GPU_CLASS_IMPLEMENT(NGARenderTargetView);
	NGA_GPU_CLASS_IMPLEMENT(NGADepthStencilView);

	bool NGAShaderResourceView::Construct(const NGATexture &texture)
	{
		NA_ASSERT_RETURN_VALUE(!IsConstructed(), false);
		NA_ASSERT_RETURN_VALUE(texture.IsConstructed(), false);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
		
		const NGATextureDesc &texDesc = texture.GetDesc();
		switch (texDesc.mType) {
		case NGATextureType::TEXTURE2D:
		{
			ID3D11Texture2D *tex;
			texture.mResource->QueryInterface(&tex);

			D3D11_TEXTURE2D_DESC texDesc;
			tex->GetDesc(&texDesc);

			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Format = texDesc.Format;
			desc.Texture2D.MipLevels = -1;
			
			tex->Release();
			break;
		}

		default:
			NA_ASSERT_RETURN_VALUE(false, false, "Unimplemented texture type.");
		};

		// Convert typeless format to strict format
		if (NGAFormatIsTypeless(texture.mDesc.mFormat)) {
			desc.Format = NGATypelessFormatToColorDXGI(texture.mDesc.mFormat);
		}
		
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

	bool NGAShaderResourceView::operator==(const NGAShaderResourceView& other)const
	{
		return mView == other.mView;
	}

	bool NGAShaderResourceView::operator!=(const NGAShaderResourceView& other)const
	{
		return mView != other.mView;
	}


	bool NGARenderTargetView::Construct(const NGATexture &texture)
	{
		NA_ASSERT_RETURN_VALUE(!IsConstructed(), false);
		NA_ASSERT_RETURN_VALUE(texture.IsConstructed(), false);

		const NGATextureDesc &textureDesc = texture.GetDesc();

		const D3D11_RTV_DIMENSION dimension =
			(textureDesc.mType == NGATextureType::TEXTURE1D) ? D3D11_RTV_DIMENSION_TEXTURE1D :
			(textureDesc.mType == NGATextureType::TEXTURE2D) ? D3D11_RTV_DIMENSION_TEXTURE2D :
			(textureDesc.mType == NGATextureType::TEXTURE3D) ? D3D11_RTV_DIMENSION_TEXTURE3D :
			D3D11_RTV_DIMENSION_UNKNOWN;

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.Format = NGAFormatToDXGI(textureDesc.mFormat);
		renderTargetViewDesc.ViewDimension = dimension;

		// Create the render target view.
		HRESULT hr = NgaDx11State.mDevice->CreateRenderTargetView(texture.mResource, &renderTargetViewDesc, &mView);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to create render target view.");

		return true;
	}

	bool NGARenderTargetView::Construct(const NGASwapChain &swapChain)
	{
		NA_ASSERT_RETURN_VALUE(!IsConstructed(), false);
		NA_ASSERT_RETURN_VALUE(swapChain.IsConstructed(), false);

		// TODO: Might be a better way to query the back buffer.
		ID3D11Texture2D *backBuffer;
		HRESULT hr = swapChain.mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to get back buffer. HRESULT %X", hr);

		hr = NgaDx11State.mDevice->CreateRenderTargetView(backBuffer, nullptr, &mView);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "ID3D11Device::CreateRenderTargetView() failed with HRESULT %X", hr);

		NGA_DX11_SAFE_RELEASE(backBuffer);

		return true;
	}

	void NGARenderTargetView::Destruct()
	{
		NGA_DX11_SAFE_RELEASE(mView);
	}

	bool NGARenderTargetView::IsConstructed()const
	{
		return mView != nullptr;
	}

	bool NGARenderTargetView::operator==(const NGARenderTargetView& other)const
	{
		return mView == other.mView;
	}


	bool NGADepthStencilView::Construct(const NGATexture& texture)
	{
		NA_ASSERT_RETURN_VALUE(!IsConstructed(), false);
		NA_ASSERT_RETURN_VALUE(texture.IsConstructed(), false);

		const NGATextureDesc &textureDesc = texture.GetDesc();
	
		D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		// Convert typeless format to strict format
		if (NGAFormatIsTypeless(textureDesc.mFormat)) {
			desc.Format = NGATypelessFormatToDepthDXGI(textureDesc.mFormat);
		}
		else {
			desc.Format = NGAFormatToDXGI(textureDesc.mFormat);
		}

		HRESULT hr = NgaDx11State.mDevice->CreateDepthStencilView(texture.mResource, &desc, &mView);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "ID3D11Device::CreateDepthStencilView() failed with HRESULT %X", hr);

		return true;
	}

	void NGADepthStencilView::Destruct()
	{
		NGA_DX11_SAFE_RELEASE(mView);
	}

	bool NGADepthStencilView::IsConstructed()const
	{
		return mView != nullptr;
	}

	bool NGADepthStencilView::operator==(const NGADepthStencilView& other)const
	{
		return mView == other.mView;
	}
}

#endif