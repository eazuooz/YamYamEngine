#pragma once
#include "yaEntity.h"
#include "yaGraphics.h"

namespace ya::graphics
{
	class StructedBuffer : public GpuBuffer
	{
	public:
		StructedBuffer();
		~StructedBuffer();

		bool Create(UINT size, UINT stride, eViewType type, void* data, bool cpuAccess = false);
		void SetData(void* data, UINT stride);
		void GetData(void* data, UINT size = 0);
		void BindSRV(eShaderStage stage, UINT slot);
		void BindUAV(UINT slot);
		void SetDescription(eViewType type);
		bool CreateBuffer(void* data);
		bool CreateView();
		bool CreateRWBuffer();
		void Clear();

		UINT GetSize() { return mSize; }
		UINT GetStride() { return mStride; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mWriteBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mReadBuffer;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> mUAV;
		eViewType mType;
		UINT mSize;
		UINT mStride;
		UINT mSRVSlot;
		UINT mUAVslot;
	};
}
