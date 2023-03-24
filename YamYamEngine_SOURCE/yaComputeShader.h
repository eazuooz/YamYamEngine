#pragma once
#include "yaResource.h"
#include "yaGraphics.h"

namespace ya::graphics
{
	class ComputeShader : public Resource
	{
	public:
		ComputeShader(UINT x, UINT y, UINT z);
		ComputeShader();
		~ComputeShader();

		virtual HRESULT Load(const std::wstring& path) override { return S_OK; }
		bool Create(const std::wstring& name, const std::string& functionName);
		void OnExcute();

		virtual void Binds();
		virtual void Clear();

	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> mCSBlob;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> mCS;

		UINT mThreadGroupCountX;
		UINT mThreadGroupCountY;
		UINT mThreadGroupCountZ;

		UINT mGroupX;
		UINT mGroupY;
		UINT mGroupZ;
	};
}
