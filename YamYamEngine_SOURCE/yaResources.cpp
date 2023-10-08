#include "yaResources.h"
#include "yaFbxLoader.h"
#include "yaFmod.h"
#include "yaFontWrapper.h"

namespace ya
{
	std::map<std::wstring, std::shared_ptr<Resource>>  Resources::mResources;

	void Resources::Initialize()
	{
		Fmod::Initialize();
		FontWrapper::Initialize();
	}

	void Resources::Release()
	{
		std::map<std::wstring, std::shared_ptr<Resource>>::iterator iter = mResources.begin();
		for (; iter != mResources.end(); ++iter)
		{
			iter->second = nullptr;
		}

		//FbxLoader::Release();
		Fmod::Release();
		FontWrapper::Release();
	}
}
