#pragma once
#include "yaResource.h"
#include "yaGraphics.h"

namespace ya
{
	class Mesh : public Resource
	{
	public:
		Mesh();
		~Mesh();

		virtual HRESULT Load(const std::wstring& path) override;

		bool CreateVertexBuffer(void* data, UINT Count);
		bool CreateIndexBuffer(void* data, UINT Count);
		bool CreateConstantBuffer(void* data, UINT Size);

		void BindBuffer();

		ID3D11Buffer* GetConstantBuffer()
		{
			return mConstantBuffer.Get();
		}



	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		D3D11_BUFFER_DESC mVBDesc;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		D3D11_BUFFER_DESC mIBDesc;
		UINT mIndexCount;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer;
		D3D11_BUFFER_DESC mCBDesc;
	};
}
