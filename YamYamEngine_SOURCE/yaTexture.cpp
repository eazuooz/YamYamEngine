#include "yaTexture.h"
#include "yaGraphicsDevice_DX11.h"



namespace ya::graphics
{
	UINT Texture::Quality = 0;
	Texture::Texture()
		: Resource(eResourceType::Texture)
		, mDesc{}
		, mTexture(nullptr)
	{

	}

	Texture::~Texture()
	{

	}



	void Texture::ClearShaderResourceView(UINT slot)
	{
		ID3D11ShaderResourceView* pSRV = nullptr;
		GetDevice()->BindShaderResource(eShaderStage::VS, slot, &pSRV);
		GetDevice()->BindShaderResource(eShaderStage::DS, slot, &pSRV);
		GetDevice()->BindShaderResource(eShaderStage::GS, slot, &pSRV);
		GetDevice()->BindShaderResource(eShaderStage::HS, slot, &pSRV);
		GetDevice()->BindShaderResource(eShaderStage::PS, slot, &pSRV);
		GetDevice()->BindShaderResource(eShaderStage::CS, slot, &pSRV);
	}

	void Texture::ClearUnorderedAccessView(UINT slot)
	{
		ID3D11UnorderedAccessView* p = nullptr;
		UINT i = -1;
		GetDevice()->BindUnorderedAccessViews(slot, &p, &i);
	}

	bool Texture::Create(UINT width, UINT height, DXGI_FORMAT format, UINT bindFlag)
	{
		mDesc.BindFlags = bindFlag;
		mDesc.Usage = D3D11_USAGE_DEFAULT;
		mDesc.CPUAccessFlags = 0;
		mDesc.Format = format;
		mDesc.Width = width;
		mDesc.Height = height;
		mDesc.ArraySize = 1;

		mDesc.SampleDesc.Count = 1;
		if (Quality > 0) 
		{
			mDesc.SampleDesc.Count = 4; // how many multisamples
			mDesc.SampleDesc.Quality = Quality - 1;
		}
		else 
		{
			mDesc.SampleDesc.Count = 1; // how many multisamples
			mDesc.SampleDesc.Quality = 0;
		}

		mDesc.MipLevels = 1;
		mDesc.MiscFlags = 0;

		if (!GetDevice()->CreateTexture(&mDesc, nullptr, mTexture.GetAddressOf()))
			return false;

		if (!CreateView(mDesc.BindFlags))
			return false;

		return true;
	}

	bool Texture::Create(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture)
	{
		mTexture = texture;
		texture->GetDesc(&mDesc);

		if (!CreateView(mDesc.BindFlags))
			return false;

		return true;
	}

	bool Texture::CreateView(UINT bindFlag)
	{
		if (bindFlag & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
		{
			if (!CreateRTV())
				return false;
		}
		if (bindFlag & (UINT)D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
		{
			if (!CreateDSV())
				return false;
		}
		if (bindFlag & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
		{
			if (!CreateSRV())
				return false;
		}
		if (bindFlag & D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
		{
			if (!CreateUAV())
				return false;
		}

		return true;
	}

	bool Texture::CreateSRV()
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = mDesc.Format;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;

		if (Quality > 0)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DMS;
			srvDesc.Texture2DMS.UnusedField_NothingToDefine = 0;
		}

		if (!GetDevice()->CreateShaderResourceView(mTexture.Get(), &srvDesc, mSRV.GetAddressOf()))
			return false;

		return true;
	}

	bool Texture::CreateUAV()
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = mDesc.Format;
		uavDesc.Texture2D.MipSlice = 0;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;

		if (!GetDevice()->CreateUnorderedAccessView(mTexture.Get(), &uavDesc, mUAV.GetAddressOf()))
			return false;

		return true;
	}

	bool Texture::CreateRTV()
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = mDesc.Format;
		rtvDesc.Texture2D.MipSlice = 0;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;


		if (!GetDevice()->CreateRenderTargetView(mTexture.Get(), &rtvDesc, mRTV.GetAddressOf()))
			return false;

		return true;
	}

	bool Texture::CreateDSV()
	{
		if (!GetDevice()->CreateDepthStencilView(mTexture.Get(), nullptr, mDSV.GetAddressOf()))
			return false;

		return true;
	}

	void Texture::Reset()
	{
		if (mTexture)
			mTexture.Reset();

		if (mRTV)
			mRTV.Reset();
		if (mDSV)
			mDSV.Reset();
		if (mUAV)
			mUAV.Reset();
		if (mSRV)
			mSRV.Reset();
	}

	HRESULT Texture::Load(const std::wstring& path)
	{
		wchar_t szExtension[50] = {};
		_wsplitpath_s(path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtension, 50);

		std::wstring extension = szExtension;
		if (extension == L".dds" || extension == L".DDS")
		{
			if (FAILED(LoadFromDDSFile(path.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, mImage)))
				return S_FALSE;
		}
		else if (extension == L".tga" || extension == L".TGA")
		{
			if (FAILED(LoadFromTGAFile(path.c_str(), nullptr, mImage)))
				return S_FALSE;
		}
		else 
		{
			if (FAILED(LoadFromWICFile(path.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, mImage)))
				return S_FALSE;
		}

		CreateShaderResourceView
		(
			GetDevice()->GetID3D11Device()
			, mImage.GetImages()
			, mImage.GetImageCount()
			, mImage.GetMetadata()
			, mSRV.GetAddressOf()
		);
		mSRV->GetResource((ID3D11Resource**)mTexture.GetAddressOf());
		mTexture->GetDesc(&mDesc);

		return S_OK;
	}

	void Texture::BindShaderResource(eShaderStage stage, UINT slot)
	{
		GetDevice()->BindShaderResource(stage, slot, mSRV.GetAddressOf());
	}

	void Texture::BindUnorderedAccessViews(UINT slot)
	{
		UINT i = -1;
		GetDevice()->BindUnorderedAccessViews(slot, mUAV.GetAddressOf(), &i);
	}


}

