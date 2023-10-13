#pragma once
#include "yaGraphics.h"
#include "yaMesh.h"

#include <fbxsdk.h>
#include <fbxsdk/scene/geometry/fbxnurbs.h >

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
	struct AnimationClip
	{
		std::wstring name;
		fbxsdk::FbxTime startTime;
		fbxsdk::FbxTime endTime;
		fbxsdk::FbxLongLong timeLength;
		fbxsdk::FbxTime::EMode mode;
	};

	class FbxLoader
	{
	public:
		FbxLoader();
		~FbxLoader();

		void Initialize();
		bool Load(const std::wstring& path);
		bool CreateMesh();
		void Release();
		
		std::vector<MeshData*>& GetMeshDatas() { return mMeshDatas; }

	private:
		void triangulate(fbxsdk::FbxNode* node);
		void loadMeshData(fbxsdk::FbxNode* node, MeshData* meshData);
		void loadMesh(fbxsdk::FbxMesh* mesh, MeshData* meshData);
		void loadVertexData(fbxsdk::FbxMesh* mesh, MeshData* meshData);
		void loadAnimationVertexData(fbxsdk::FbxMesh* mesh, MeshData* meshData);
		void loadMaterial(fbxsdk::FbxSurfaceMaterial* material, MeshData* meshData);
		void loadTextures();
		void loadAnimationClips(fbxsdk::FbxScene* scene);
		void loadBoneData(fbxsdk::FbxNode* node, int depth, int idx, int parentIdx);
		void CreateMaterial();

		void getPosition(fbxsdk::FbxMesh* mesh, MeshData* meshData);
		void getTangent(fbxsdk::FbxMesh* mesh, MeshData* meshData, int idx, int order);
		void getBinormal(fbxsdk::FbxMesh* mesh, MeshData* meshData, int idx, int order);
		void getNormal(fbxsdk::FbxMesh* mesh, MeshData* meshData, int idx, int order);
		void getUV(fbxsdk::FbxMesh* mesh, MeshData* meshData, int idx, int order);
		Matrix getMatrixFromFbxMatrix(fbxsdk::FbxAMatrix& matrix);
		
		int getBoneIndex(std::string name);
		fbxsdk::FbxAMatrix GetTransformMatrix(fbxsdk::FbxNode* node);
		void LoadWeightsAndIndices(fbxsdk::FbxCluster* cluster, int boneIdx, MeshData* meshData);
		void LoadOffsetMatrix(fbxsdk::FbxCluster* cluster, const fbxsdk::FbxAMatrix& transform, int boneIdx, MeshData* meshData);
		void LoadKeyframeTransform(fbxsdk::FbxNode* node, fbxsdk::FbxCluster* cluster, fbxsdk::FbxAMatrix& transform, int boneIdx, MeshData* meshData);
		void loadWeightsAndIndices(fbxsdk::FbxMesh* _pMesh, MeshData* meshData);

		Vector4 GetMaterialColor(fbxsdk::FbxSurfaceMaterial* material, const char* type, const char* typeFactor);
		std::wstring GetMaterialTextureName(fbxsdk::FbxSurfaceMaterial* material, const char* type);

	private:
		fbxsdk::FbxManager* mManager;
		fbxsdk::FbxScene* mScene;
		fbxsdk::FbxImporter* mImporter;

		std::vector<MeshData*> mMeshDatas;
		std::vector<MeshData::Bone*> mBones;
		std::vector<AnimationClip*> mAnimationClips;
		
	};
}



