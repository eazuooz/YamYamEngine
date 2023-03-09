#pragma once
#include "yaResource.h"
#include "yaGraphics.h"

namespace ya::graphics
{
	class ComputeShader : public Resource
	{
	public:
		ComputeShader();
		~ComputeShader();

		bool Create(const std::wstring& name, const std::string& functionName);

	private:
		Microsoft::WRL::ComPtr<ID3DBlob> mCSBlob;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> mCS;
	};
}
