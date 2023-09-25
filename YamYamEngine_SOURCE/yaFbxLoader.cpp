#include "yaFbxLoader.h"
#include <filesystem>

namespace ya
{
	FbxManager* FbxLoader::mManager = nullptr;

	void FbxLoader::Initialize()
	{
		mManager = FbxManager::Create();
		fbxsdk::FbxIOSettings* ioSettings
			= fbxsdk::FbxIOSettings::Create(mManager, IOSROOT);
		mManager->SetIOSettings(ioSettings);
	}

	bool FbxLoader::Load(const std::wstring& path)
	{
		fbxsdk::FbxScene* scene = fbxsdk::FbxScene::Create(mManager, "");
		fbxsdk::FbxImporter* importer = fbxsdk::FbxImporter::Create(mManager, "");

		
		std::filesystem::path currentDirectory = std::filesystem::current_path();
		currentDirectory = currentDirectory.parent_path();
		currentDirectory += L"\\Resources\\fbx\\";
		currentDirectory += path;

		std::string cpath(currentDirectory.string());
		if (!importer->Initialize(cpath.c_str(), -1, mManager->GetIOSettings()))
			return false;

		importer->Import(scene);
		scene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::Max);

		fbxsdk::FbxNode* rootNode = scene->GetRootNode();
		triangulate(rootNode);
		loadMeshData(rootNode);

		return true;
	}

	void FbxLoader::triangulate(fbxsdk::FbxNode* node)
	{
		fbxsdk::FbxNodeAttribute* attribute = node->GetNodeAttribute();
		if (attribute)
		{
			fbxsdk::FbxNodeAttribute::EType type = attribute->GetAttributeType();
			if (type == fbxsdk::FbxNodeAttribute::eMesh
				|| type == fbxsdk::FbxNodeAttribute::eNurbs
				|| type == fbxsdk::FbxNodeAttribute::eNurbsSurface)
			{
				fbxsdk::FbxGeometryConverter converter(mManager);
				converter.Triangulate(attribute, true);
			}
		}

		int childCount = node->GetChildCount();
		for (int i = 0; i < childCount; ++i)
			triangulate(node->GetChild(i));
	}

	void FbxLoader::loadMeshData(fbxsdk::FbxNode* node)
	{
		fbxsdk::FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
		if (nodeAttribute && fbxsdk::FbxNodeAttribute::eMesh == nodeAttribute->GetAttributeType())
		{
			fbxsdk::FbxAMatrix globalMatrix = node->EvaluateGlobalTransform();
			FbxVector4 t = globalMatrix.GetT();
			FbxVector4 r = globalMatrix.GetR();
			FbxVector4 s = globalMatrix.GetS();

			fbxsdk::FbxMesh* mesh = node->GetMesh();
			if (NULL != mesh)
				loadMesh(mesh);
		}

		UINT materialCount = node->GetMaterialCount();
		if (materialCount > 0)
		{
			for (UINT i = 0; i < materialCount; ++i)
			{
				fbxsdk::FbxSurfaceMaterial* surface = node->GetMaterial(i);
				loadMaterial(surface);
			}
		}

		int childCount = node->GetChildCount();
		for (int i = 0; i < childCount; ++i)
			loadMeshData(node->GetChild(i));
	}

	void FbxLoader::loadMesh(fbxsdk::FbxMesh* mesh)
	{
		UINT vtxCount = mesh->GetPolygonSize(0);
		if (vtxCount != 3)
		{
			// 구성 정점이 3개가 아닌경우
			return;
		}

		MeshData meshData = {};

		std::string cName = mesh->GetName();
		meshData.name = std::wstring(cName.begin(), cName.end());

		//vertex
		getPosition(mesh, meshData);

		UINT materialCount = mesh->GetNode()->GetMaterialCount();
		meshData.indices.resize(materialCount);

		//UINT polygonCount = mesh->GetPolygonCount();
		UINT order = 0;
		UINT indices[3] = {};
		fbxsdk::FbxGeometryElementMaterial* elementMaterial = mesh->GetElementMaterial();
		for (size_t i = 0; i < mesh->GetPolygonCount(); i++)
		{
			int idx = mesh->GetPolygonVertex(i, 0);
			indices[0] = idx;
			getTangent(mesh, idx, order);
			getBinormal(mesh, idx, order);
			getNormal(mesh, idx, order);
			getUV(mesh, idx, mesh->GetTextureUVIndex(i, 0));
			order++;

			idx = mesh->GetPolygonVertex(i, 1);
			indices[1] = idx;
			getTangent(mesh, idx, order);
			getBinormal(mesh, idx, order);
			getNormal(mesh, idx, order);
			getUV(mesh, idx, mesh->GetTextureUVIndex(i, 1));
			order++;

			idx = mesh->GetPolygonVertex(i, 2);
			indices[2] = idx;
			getTangent(mesh, idx, order);
			getBinormal(mesh, idx, order);
			getNormal(mesh, idx, order);
			getUV(mesh, idx, mesh->GetTextureUVIndex(i, 2));
			order++;

			UINT subset = elementMaterial->GetIndexArray().GetAt(i);
			//meshData.indices[subset].push_back(indices[0]);
			//meshData.indices[subset].push_back(indices[2]);
			//meshData.indices[subset].push_back(indices[1]);
		}



		

	}

	void FbxLoader::loadMaterial(fbxsdk::FbxSurfaceMaterial* material)
	{

	}

	void FbxLoader::getPosition(fbxsdk::FbxMesh* mesh, MeshData& meshData)
	{
		UINT vtxCount = mesh->GetControlPointsCount();
		meshData.vertices.resize(vtxCount);
		fbxsdk::FbxVector4* positions = mesh->GetControlPoints();
		for (size_t i = 0; i < vtxCount; i++)
		{
			meshData.vertices[i].pos.x = static_cast<float>(positions[i].mData[0]);
			meshData.vertices[i].pos.y = static_cast<float>(positions[i].mData[2]);
			meshData.vertices[i].pos.z = static_cast<float>(positions[i].mData[1]);
		}
	}

	Vector3 FbxLoader::getTangent(fbxsdk::FbxMesh* mesh, int idx, int order)
	{
		return Vector3();
	}

	Vector3 FbxLoader::getBinormal(fbxsdk::FbxMesh* mesh, int idx, int order)
	{
		return Vector3();
	}

	Vector3 FbxLoader::getNormal(fbxsdk::FbxMesh* mesh, int idx, int order)
	{
		return Vector3();
	}

	Vector2 FbxLoader::getUV(fbxsdk::FbxMesh* mesh, int idx, int order)
	{
		return Vector2();
	}

	void FbxLoader::Release()
	{
		mManager->Destroy();
	}

}
