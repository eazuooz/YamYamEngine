#pragma once
#include "CommonInclude.h"
#include "YamYamEngine.h"
#include "yaTexture.h"

namespace ya
{
	class Resource;
	class Resources
	{
	public:
		template <typename T>
		static std::shared_ptr<T> Find(const std::wstring& key)
		{
			std::map<std::wstring, std::shared_ptr<Resource>>::iterator iter = mResources.find(key);

			// 이미 동일한 키값으로 다른 리소스가 먼저 등록되어 있었다.
			if (iter != mResources.end())
			{
				return std::dynamic_pointer_cast<T>(iter->second);
			}

			return nullptr;
		}

		template <typename T>
		static std::shared_ptr<T> Find(const std::string& key)
		{
			std::wstring wKey(key.begin(), key.end());
			return Find<T>(wKey);
		}

		template <typename T>
		static std::vector<std::shared_ptr<T>> Finds()
		{
			std::vector<std::shared_ptr<T>> resources;
			for (auto iter : mResources)
			{
				std::shared_ptr<T> resource = std::dynamic_pointer_cast<T>(iter.second);

				if (resource)
					resources.push_back(resource);
			}

			return resources;
		}

		template <typename T>
		static std::shared_ptr<T> Load(const std::wstring& key, const std::wstring& path)
		{
			// 키값으로 탐색
			std::shared_ptr<T> resource = Resources::Find<T>(key);
			if (nullptr != resource)
			{
				// 해당키로 이미 로딩된게 있으면 해당 리소스를 반환
				return resource;
			}

			// 해당 키로 로딩된 리소스가 없다.
			resource = std::make_shared<T>();
			if (FAILED(resource->Load(path)))
			{
				MessageBox(nullptr, L"Image Load Failed!", L"Error", MB_OK);
				return nullptr;
			}

			resource->SetKey(key);
			resource->SetName(key);
			resource->SetPath(path);
			mResources.insert(std::make_pair(key, std::dynamic_pointer_cast<Resource>(resource)));

			return resource;
		}

		//template <typename T>
		static void Release(const std::wstring& key)
		{
			std::map<std::wstring, std::shared_ptr<Resource>>::iterator iter = mResources.find(key);

			// 이미 동일한 키값으로 다른 리소스가 먼저 등록되어 있었다.
			if (iter == mResources.end())
				return;
			iter->second.reset();
			iter->second = nullptr;

			mResources.erase(key);
		}

		template <typename T>
		static void Insert(const std::wstring& key, std::shared_ptr<T> resource)
		{
			resource->SetKey(key);
			resource->SetName(key);
			mResources.insert(std::make_pair(key, std::dynamic_pointer_cast<Resource>(resource)));
		}

		static void Release()
		{
			std::map<std::wstring, std::shared_ptr<Resource>>::iterator iter = mResources.begin();
			for (; iter != mResources.end(); ++iter)
			{
				iter->second = nullptr;
			}
		}

	private:
		Resources() = delete;
		~Resources() = delete;

	private:
		static std::map<std::wstring, std::shared_ptr<Resource>> mResources;
	};


}
