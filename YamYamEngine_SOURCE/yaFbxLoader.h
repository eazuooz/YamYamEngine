#pragma once
#include "yaGraphics.h"
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
		static bool Load(const std::wstring& path);
		static void Triangulate(fbxsdk::FbxNode* node);
		
		static void Release();
		

	private:
		/**
		 * \brief 
		 */
		static fbxsdk::FbxManager* mManager;
	};
}



