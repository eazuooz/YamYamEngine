#pragma once
#include "yaResource.h"
#include "yaGraphics.h"

namespace ya::graphics
{
	class Shader : public Resource
	{
	public:
		Shader();
		~Shader();

		virtual HRESULT Load(const std::wstring& path) override;

		void Create(const graphics::eShaderStage stage, const std::wstring& file, const std::string& funcName);
		void Bind();


		ID3D11InputLayout* GetInputLayout()
		{
			return mInputLayout.Get();
		}
		ID3D11InputLayout** GetInputLayoutAddressOf()
		{
			return mInputLayout.GetAddressOf();
		}

		ID3DBlob* GetVSCode()
		{
			return mVSBlob.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		D3D11_PRIMITIVE_TOPOLOGY mTopology;
		graphics::eShaderStage mStage;
		
		Microsoft::WRL::ComPtr<ID3DBlob> mVSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> mHSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> mDSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> mGSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> mPSBlob;
		
		Microsoft::WRL::ComPtr<ID3DBlob> mErrorBlob;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVS;
		Microsoft::WRL::ComPtr<ID3D11HullShader> mHS;
		Microsoft::WRL::ComPtr<ID3D11DomainShader> mDS;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGS;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPS;
	};
}
