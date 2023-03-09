#include "yaShader.h"
#include <filesystem>
#include "yaGraphicsDevice_DX11.h"
#include "yaRenderer.h"

namespace ya::graphics
{
	Shader::Shader()
		: Resource(eResourceType::GraphicsShader)
		, mTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, mRSType(eRSType::SolidBack)
		, mDSType(eDSType::Less)
		, mBSType(eBSType::Default)
	{
	}
	Shader::~Shader()
	{
	}
	HRESULT Shader::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}
	void Shader::Create(const graphics::eShaderStage stage, const std::wstring& file, const std::string& funcName)
	{
		std::filesystem::path path 
			= std::filesystem::current_path().parent_path();
		path += "\\Shaders_SOURCE\\";

		std::wstring shaderPath(path);
		shaderPath += file;

		ID3DBlob* errorBlob = nullptr;
		if (stage == graphics::eShaderStage::VS)
		{
			graphics::GetDevice()->CompileFromFile(file, funcName, "vs_5_0", mVSBlob.GetAddressOf());
			graphics::GetDevice()->CreateVertexShader(mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), mVS.GetAddressOf());
		}
		
		if (stage == graphics::eShaderStage::PS)
		{
			graphics::GetDevice()->CompileFromFile(file, funcName, "ps_5_0", mPSBlob.GetAddressOf());
			graphics::GetDevice()->CreatePixelShader(mPSBlob->GetBufferPointer(), mPSBlob->GetBufferSize(), mPS.GetAddressOf());
		}
		
	}

	void Shader::Bind()
	{
		GetDevice()->BindPrimitiveTopology(mTopology);
		GetDevice()->BindInputLayout(mInputLayout.Get());

		GetDevice()->BindVertexShader(mVS.Get());
		GetDevice()->BindPixelShader(mPS.Get());

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rsState = renderer::rasterizeStates[(UINT)mRSType];
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState = renderer::depthStencilStates[(UINT)mDSType];
		Microsoft::WRL::ComPtr<ID3D11BlendState> bsState = renderer::blendStateStates[(UINT)mBSType];

		GetDevice()->BindRasterizerState(rsState.Get());
		GetDevice()->BindDepthStencilState(dsState.Get());
		GetDevice()->BindBlendState(bsState.Get());
	}
}
