#pragma once
#include "yaGraphics.h"
#include "yaMeshData.h"
#include <fbxsdk.h>

#ifdef _DEBUG
//#pragma comment(lib, "..\\External\\Library\\Fbx\\Debug\\libfbxsdk.lib")
#pragma comment(lib, "..\\External\\Library\\Fbx\\Debug\\libfbxsdk-md.lib")
#pragma comment(lib, "..\\External\\Library\\Fbx\\Debug\\libxml2-md.lib")
#pragma comment(lib, "..\\External\\Library\\Fbx\\Debug\\zlib-md.lib")
#else
//#pragma comment(lib, "..\\External\\Library\\Fbx\\Debug\\libfbxsdk.lib")
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
		static void Release();
		
	private:
		static void triangulate(fbxsdk::FbxNode* node);
		static void loadMeshData(fbxsdk::FbxNode* node);
		static void loadMesh(fbxsdk::FbxMesh* mesh);
		static void loadMaterial(fbxsdk::FbxSurfaceMaterial* material);

		static void getPosition(fbxsdk::FbxMesh* mesh, MeshData& meshData);
		static Vector3 getTangent(fbxsdk::FbxMesh* mesh, int idx, int order);
		static Vector3 getBinormal(fbxsdk::FbxMesh* mesh, int idx, int order);
		static Vector3 getNormal(fbxsdk::FbxMesh* mesh, int idx, int order);
		static Vector2 getUV(fbxsdk::FbxMesh* mesh, int idx, int order);

	private:
		/**
		 * \brief 
		 */
		static fbxsdk::FbxManager* mManager;
	};
}



