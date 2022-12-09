#pragma once
#include "yaResource.h"


namespace ya
{
	class Mesh : public Resource
	{
	public:
		Mesh();
		~Mesh();

		virtual HRESULT Load(const std::wstring& path) override;

		void BindBuffer();

	private:


	};
}
