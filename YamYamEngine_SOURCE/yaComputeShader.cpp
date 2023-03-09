#include "yaComputeShader.h"
#include "yaGraphicsDevice_DX11.h"

namespace ya::graphics
{
	ComputeShader::ComputeShader()
		: Resource(eResourceType::ComputeShader)
	{
	}

	ComputeShader::~ComputeShader()
	{
	}

	bool ComputeShader::Create(const std::wstring& name, const std::string& functionName)
	{
		std::filesystem::path path
			= std::filesystem::current_path().parent_path();
		path += "\\Shaders_SOURCE\\";

		std::wstring shaderPath(path);
		shaderPath += name;

		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
		if (FAILED(D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, functionName.c_str(), "cs_5_0", 0, 0, mCSBlob.GetAddressOf(), errorBlob.GetAddressOf())))
			return false;

		if (!GetDevice()->CreateComputeShader(mCSBlob->GetBufferPointer(), mCSBlob->GetBufferSize(), mCS.GetAddressOf()))
			return false;

		return true;
	}

}
