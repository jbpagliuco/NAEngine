#include "NGA/NGAResourceViews.h"

#if defined(NGA_D3D11)

#include "NGACoreInternalDX11.h"

#include "NGA/NGAResources.h"
#include "NGA/NGASwapChain.h"

namespace na
{
	NGA_GPU_CLASS_IMPLEMENT(NGAShaderResourceView);
	NGA_GPU_CLASS_IMPLEMENT(NGARenderTargetView);

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



	bool NGADepthStencilView::Construct(const NGADepthStencilViewDesc &desc)
	{
		NA_ASSERT_RETURN_VALUE(!IsConstructed(), false);
		NA_ASSERT_RETURN_VALUE(desc.mFormat != NGADepthBufferFormat::NONE, false);

		D3D11_TEXTURE2D_DESC texDesc{};
		texDesc.Width = desc.mWidth;
		texDesc.Height = desc.mWidth;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = NGAFormatToDXGI(desc.mFormat);
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		HRESULT hr = NgaDx11State.mDevice->CreateTexture2D(&texDesc, nullptr, &mBuffer);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "Failed to create depth stencil buffer. HRESULT %X", hr);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Format = texDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		hr = NgaDx11State.mDevice->CreateDepthStencilView(mBuffer, &depthStencilViewDesc, &mView);
		NA_ASSERT_RETURN_VALUE(SUCCEEDED(hr), false, "ID3D11Device::CreateDepthStencilView() failed with HRESULT %X", hr);

		return true;
	}

	void NGADepthStencilView::Destruct()
	{
		NGA_DX11_SAFE_RELEASE(mView);
		NGA_DX11_SAFE_RELEASE(mBuffer);
	}

	bool NGADepthStencilView::IsConstructed()const
	{
		return mView != nullptr;
	}
}

#endif