#pragma once
#include "yaEntity.h"
#include "yaGraphics.h"

namespace ya::graphics
{
	class ConstantBuffer : public GpuBuffer
	{
	public:
		ConstantBuffer();
		~ConstantBuffer();

		bool Create(size_t size);
		void Bind(void* data);
		void SetPipline(ShaderStage stage);

	private:
		const CBTYPES mType;
	};
}