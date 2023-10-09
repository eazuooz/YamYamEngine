#pragma once
#include "yaGraphics.h"
#include "yaMesh.h"

#include <fbxsdk.h>

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\Library\\Fbx\\Debug\\libfbxsdk-md.lib")
#pragma comment(lib, "..\\External\\Library\\Fbx\\Debug\\libxml2-md.lib")
#pragma comment(lib, "..\\External\\Library\\Fbx\\Debug\\zlib-md.lib")
#else
#pragma comment(lib, "..\\External\\Library\\Fbx\\Release\\libfbxsdk-mt.lib")
#pragma comment(lib, "..\\External\\Library\\Fbx\\Release\\libxml2-mt.lib")
#pragma comment(lib, "..\\External\\Library\\Fbx\\Release\\zlib-mt.lib")
#endif

namespace ya
{
	class FbxLoader
	{
	public:
		FbxLoader();
		~FbxLoader();

		void Initialize();
		bool Load(const std::wstring& path);
		bool CreateMesh();
		void Release();
		
		std::vector<MeshData>& GetMeshDatas() { return mMeshDatas; }

	private:
		void triangulate(fbxsdk::FbxNode* node);
		void loadMeshData(fbxsdk::FbxNode* node);
		void loadMesh(fbxsdk::FbxMesh* mesh, MeshData& meshData);
		void loadMaterial(fbxsdk::FbxSurfaceMaterial* material, MeshData& meshData);
		void loadVertexData(fbxsdk::FbxMesh* mesh, MeshData& meshData);
		void loadTextures();
		void CreateMaterial();

		void getPosition(fbxsdk::FbxMesh* mesh, MeshData& meshData);
		void getTangent(fbxsdk::FbxMesh* mesh, MeshData& meshData, int idx, int order);
		void getBinormal(fbxsdk::FbxMesh* mesh, MeshData& meshData, int idx, int order);
		void getNormal(fbxsdk::FbxMesh* mesh, MeshData& meshData, int idx, int order);
		void getUV(fbxsdk::FbxMesh* mesh, MeshData& meshData, int idx, int order);

		Vector4 GetMaterialColor(fbxsdk::FbxSurfaceMaterial* material, const char* type, const char* typeFactor);
		std::wstring GetMaterialTextureName(fbxsdk::FbxSurfaceMaterial* material, const char* type);

	private:
		fbxsdk::FbxManager* mManager;
		std::vector<MeshData> mMeshDatas;
	};
}



