#pragma once
#include "yaEntity.h"

namespace ya
{
	using namespace enums;

	class Resource : public Entity
	{
	public:
		Resource(eResourceType type);
		virtual ~Resource();

		virtual HRESULT Load(const std::wstring& path) = 0;

		const std::wstring& GetKey() { return mKey; }
		const std::wstring& GetPath() { return mPath; }

		void SetKey(const std::wstring& key) { mKey = key; }
		void SetPath(const std::wstring& path) { mPath = path; }

	private:
		const eResourceType mType;
		std::wstring mKey;
		std::wstring mPath;
	};
}
