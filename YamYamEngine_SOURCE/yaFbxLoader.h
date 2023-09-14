#pragma once
#include <fbxsdk.h>

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\Library\\Fbx\\Debug\\libfbxsdk.lib")
#pragma comment(lib, "..\\External\\Library\\Fbx\\Debug\\libfbxsdk-md.lib")
#pragma comment(lib, "..\\External\\Library\\Fbx\\Debug\\libxml2-md.lib")
#pragma comment(lib, "..\\External\\Library\\Fbx\\Debug\\zlib-md.lib")
#else
#pragma comment(lib, "..\\External\\Library\\Fbx\\Debug\\libfbxsdk.lib")
#pragma comment(lib, "..\\External\\Library\\Fbx\\Release\\libfbxsdk-mt.lib")
#pragma comment(lib, "..\\External\\Library\\Fbx\\Release\\libxml2-mt.lib")
#pragma comment(lib, "..\\External\\Library\\Fbx\\Release\\zlib-mt.lib")
#endif

namespace ya
{
	class FbxLoader
	{
	public:
		static void Initialize();
		static void Release();
		
	private:
		static FbxManager* mManger;
	};
}



