#include "yaFbxLoader.h"


namespace ya
{
	FbxManager* FbxLoader::mManger = nullptr;

	void FbxLoader::Initialize()
	{
		mManger = FbxManager::Create();


	}

	void FbxLoader::Release()
	{
		mManger->Destroy();
	}

}
