#include "yaShader.h"
#include <filesystem>
#include "yaGraphicsDevice_DX11.h"

namespace ya::graphics
{
	Shader::Shader()
		: Resource(eResourceType::GraphicsShader)
		, mTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
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
	}
}
