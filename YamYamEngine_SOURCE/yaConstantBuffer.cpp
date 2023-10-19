#include "yaConstantBuffer.h"
#include "yaGraphicsDevice_DX11.h"

namespace ya::graphics
{
	ConstantBuffer::ConstantBuffer(eCBType type)
		:mType(type)
	{

	}
	ConstantBuffer::~ConstantBuffer()
	{
	}
	bool ConstantBuffer::Create(size_t size)
	{
		desc.ByteWidth = size; // 16 의 배수로
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		if (!GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf()))
			return false;

		return true;
	}

	void ConstantBuffer::SetData(void* data)
	{
		GetDevice()->SetData(buffer.Get(), data, desc.ByteWidth);
	}

	void ConstantBuffer::Bind(eShaderStage stage)
	{
		if (stage == eShaderStage::ALL)
		{
			GetDevice()->Bind(eShaderStage::VS, mType, buffer.Get());
			GetDevice()->Bind(eShaderStage::HS, mType, buffer.Get());
			GetDevice()->Bind(eShaderStage::DS, mType, buffer.Get());
			GetDevice()->Bind(eShaderStage::GS, mType, buffer.Get());
			GetDevice()->Bind(eShaderStage::PS, mType, buffer.Get());
			GetDevice()->Bind(eShaderStage::CS, mType, buffer.Get());
		}
		else
		{
			GetDevice()->Bind(stage, mType, buffer.Get());
		}
	
	}
}

