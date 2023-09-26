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
		static void loadMesh(fbxsdk::FbxMesh* mesh, MeshData& meshData);
		static void loadMaterial(fbxsdk::FbxSurfaceMaterial* material, MeshData& meshData);
		static void loadVertexData(fbxsdk::FbxMesh* mesh, MeshData& meshData);

		static void getPosition(fbxsdk::FbxMesh* mesh, MeshData& meshData);
		static void getTangent(fbxsdk::FbxMesh* mesh, MeshData& meshData, int idx, int order);
		static void getBinormal(fbxsdk::FbxMesh* mesh, MeshData& meshData, int idx, int order);
		static void getNormal(fbxsdk::FbxMesh* mesh, MeshData& meshData, int idx, int order);
		static void getUV(fbxsdk::FbxMesh* mesh, MeshData& meshData, int idx, int order);

		static Vector4 GetMaterialColor(fbxsdk::FbxSurfaceMaterial* material, const char* type, const char* typeFactor);
		static std::wstring GetMaterialTextureName(fbxsdk::FbxSurfaceMaterial* material, const char* type);

	private:
		/**
		 * \brief 
		 */
		static fbxsdk::FbxManager* mManager;
		static std::vector<MeshData> mMeshDatas;
	};
}



