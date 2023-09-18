#include "yaFbxLoader.h"


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

		std::string cpath(path.begin(), path.end());
		if (!importer->Initialize(cpath.c_str(), -1, mManager->GetIOSettings()))
			return false;

		importer->Import(scene);
		scene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::Max);

		Triangulate(scene->GetRootNode());

		return true;
	}

	void FbxLoader::Triangulate(fbxsdk::FbxNode* node)
	{
		fbxsdk::FbxNodeAttribute* attribute = node->GetNodeAttribute();
		if (attribute == nullptr)
			return;

		fbxsdk::FbxNodeAttribute::EType type = attribute->GetAttributeType();
		if (type == fbxsdk::FbxNodeAttribute::eMesh
				|| type == fbxsdk::FbxNodeAttribute::eNurbs
				|| type == fbxsdk::FbxNodeAttribute::eNurbsSurface)
		{
			fbxsdk::FbxGeometryConverter converter(mManager);
			converter.Triangulate(attribute, true);
		}

		int childCount = node->GetChildCount();
		for (int i = 0; i < childCount; ++i)
		{
			Triangulate(node->GetChild(i));
		}
	}

	void FbxLoader::Release()
	{
		mManager->Destroy();
	}

}
