#include "yaFbxLoader.h"
#include "yaResources.h"
#include "yaTexture.h"
#include "yaMaterial.h"

#include <filesystem>

namespace ya
{
	FbxManager* FbxLoader::mManager = nullptr;
	std::vector<MeshData> FbxLoader::mMeshDatas = {};

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
		mMeshDatas.clear();
		triangulate(rootNode);
		loadMeshData(rootNode);
		loadTextures();
		CreateMaterial();
		
		return true;
	}

	std::shared_ptr<graphics::Mesh> FbxLoader::CreateMesh()
	{
		std::shared_ptr<graphics::Mesh> mesh = nullptr;
		for (MeshData& meshData : mMeshDatas)
		{
			UINT vtxCount = meshData.vertices.size();
			D3D11_BUFFER_DESC desc = {};

			//desc.ByteWidth 

		}

		return mesh;
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
		MeshData meshData = {};
		fbxsdk::FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
		if (nodeAttribute && fbxsdk::FbxNodeAttribute::eMesh == nodeAttribute->GetAttributeType())
		{
			fbxsdk::FbxAMatrix globalMatrix = node->EvaluateGlobalTransform();
			FbxVector4 t = globalMatrix.GetT();
			FbxVector4 r = globalMatrix.GetR();
			FbxVector4 s = globalMatrix.GetS();

			fbxsdk::FbxMesh* mesh = node->GetMesh();
			if (NULL != mesh)
				loadMesh(mesh, meshData);
		}

		UINT materialCount = node->GetMaterialCount();
		if (materialCount > 0)
		{
			for (UINT i = 0; i < materialCount; ++i)
			{
				fbxsdk::FbxSurfaceMaterial* surface = node->GetMaterial(i);
				loadMaterial(surface, meshData);
			}
		}

		if (nodeAttribute)
			mMeshDatas.push_back(meshData);

		int childCount = node->GetChildCount();
		for (int i = 0; i < childCount; ++i)
			loadMeshData(node->GetChild(i));
	}

	void FbxLoader::loadMesh(fbxsdk::FbxMesh* mesh, MeshData& meshData)
	{
		UINT vtxCount = mesh->GetPolygonSize(0);
		if (vtxCount != 3)
		{
			// 구성 정점이 3개가 아닌경우
			return;
		}

		std::string cName = mesh->GetName();
		meshData.name = std::wstring(cName.begin(), cName.end());

		loadVertexData(mesh, meshData);

		//load animation data


		

	}

	void FbxLoader::loadMaterial(fbxsdk::FbxSurfaceMaterial* material, MeshData& meshData)
	{
		MeshData::MaterialData materialInfo{};

		std::string str = material->GetName();
		materialInfo.name = std::wstring(str.begin(), str.end());

		// Diff
		materialInfo.diffuseColor = GetMaterialColor(material
			, fbxsdk::FbxSurfaceMaterial::sDiffuse
			, fbxsdk::FbxSurfaceMaterial::sDiffuseFactor);

		// Amb
		materialInfo.ambientColor = GetMaterialColor(material
			, fbxsdk::FbxSurfaceMaterial::sAmbient
			, fbxsdk::FbxSurfaceMaterial::sAmbientFactor);

		// Spec
		materialInfo.specularColor = GetMaterialColor(material
			, fbxsdk::FbxSurfaceMaterial::sSpecular
			, fbxsdk::FbxSurfaceMaterial::sSpecularFactor);

		// Emisv
		materialInfo.emessiveColor = GetMaterialColor(material
			, fbxsdk::FbxSurfaceMaterial::sEmissive
			, fbxsdk::FbxSurfaceMaterial::sEmissiveFactor);

		// Texture Name
		materialInfo.diffuse = GetMaterialTextureName(material, fbxsdk::FbxSurfaceMaterial::sDiffuse);
		materialInfo.normal = GetMaterialTextureName(material, fbxsdk::FbxSurfaceMaterial::sNormalMap);
		materialInfo.specular = GetMaterialTextureName(material, fbxsdk::FbxSurfaceMaterial::sSpecular);
		materialInfo.emessive = GetMaterialTextureName(material, fbxsdk::FbxSurfaceMaterial::sEmissive);

		meshData.materials.push_back(materialInfo);
	}

	void FbxLoader::loadVertexData(fbxsdk::FbxMesh* mesh, MeshData& meshData)
	{
		//vertex
		getPosition(mesh, meshData);

		UINT materialCount = mesh->GetNode()->GetMaterialCount();
		meshData.indices2.resize(materialCount);

		//UINT polygonCount = mesh->GetPolygonCount();
		UINT order = 0;
		UINT indices[3] = {};
		fbxsdk::FbxGeometryElementMaterial* elementMaterial = mesh->GetElementMaterial();
		for (size_t i = 0; i < mesh->GetPolygonCount(); i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				int idx = mesh->GetPolygonVertex(i, j);
				indices[j] = idx;
				getTangent(mesh, meshData, idx, order);
				getBinormal(mesh, meshData, idx, order);
				getNormal(mesh, meshData, idx, order);
				getUV(mesh, meshData, idx, mesh->GetTextureUVIndex(i, j));
				order++;
			}

			UINT subset = elementMaterial->GetIndexArray().GetAt(i);
			meshData.indices2[subset].push_back(indices[0]);
			meshData.indices2[subset].push_back(indices[2]);
			meshData.indices2[subset].push_back(indices[1]);
		}
	}

	void FbxLoader::loadTextures()
	{
		// 텍스처 로드
		std::filesystem::path parentPath = std::filesystem::current_path().parent_path();
		std::filesystem::path path_fbx_texture = parentPath.wstring() + L"\\Resources\\fbx\\Texture\\";

		std::filesystem::path path_origin;
		std::filesystem::path path_filename;
		std::filesystem::path path_dest;

		for (MeshData& data : mMeshDatas)
		{
			for (MeshData::MaterialData& matData : data.materials)
			{



				std::vector<std::filesystem::path> paths;
				paths.push_back(matData.diffuse.c_str());
				paths.push_back(matData.normal.c_str());
				paths.push_back(matData.specular.c_str());
				paths.push_back(matData.emessive.c_str());

				for (size_t i = 0; i < paths.size(); ++i)
				{
					path_origin = paths[i];
					path_filename = paths[i].filename();
					if (path_filename == L"")
						continue;

					path_dest = path_fbx_texture.wstring() + path_filename.wstring();

					if (false == exists(path_dest))
						copy(path_origin, path_dest);

					std::wstring texPath = path_dest;
					Resources::Load<graphics::Texture>(path_filename, texPath);

					switch (i)
					{
					case 0: matData.diffuse = path_filename; break;
					case 1: matData.normal = path_filename; break;
					case 2: matData.specular = path_filename; break;
					case 3: matData.emessive = path_filename; break;
					}
				}
			}

			path_origin = path_origin.parent_path();
			remove_all(path_origin);
		}
	}

	void FbxLoader::CreateMaterial()
	{
		// 메테리얼 로드
		std::wstring key;
		std::wstring path;

		for (MeshData& data : mMeshDatas)
		{
			for (MeshData::MaterialData& matData : data.materials)
			{
				std::shared_ptr<graphics::Material> material = std::make_shared<graphics::Material>();

				// Material 이름짓기
				key = matData.name;
				if (key.empty())
					key = std::filesystem::path(matData.diffuse).stem();

				path = L"material\\";
				path += key;

				// 상대경로가 곧 이름(확장자명은 제외)
				matData.name = key;
				material->SetKey(key);
				material->SetPath(path + L".mtrl");

				std::shared_ptr<graphics::Shader> phong = Resources::Find<graphics::Shader>(L"PhongShader");
				material->SetShader(phong);

				std::wstring strTexKey = matData.diffuse;
				std::shared_ptr<graphics::Texture> pTex = Resources::Find<graphics::Texture>(strTexKey);
				if (NULL != pTex)
					material->SetTexture(graphics::eTextureType::Albedo, pTex);

				strTexKey = matData.normal;
				pTex = Resources::Find<graphics::Texture>(strTexKey);
				if (NULL != pTex)
					material->SetTexture(graphics::eTextureType::Normal, pTex);

				strTexKey = matData.specular;
				pTex = Resources::Find<graphics::Texture>(strTexKey);
				if (NULL != pTex)
					material->SetTexture(graphics::eTextureType::Specular, pTex);

				material->SetColor(matData.diffuseColor, matData.specularColor, matData.ambientColor);

				material->SetRenderingMode(graphics::eRenderingMode::Opaque);
				Resources::Insert<graphics::Material>(material->GetKey(), material);
			}
		}
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

	void FbxLoader::getTangent(fbxsdk::FbxMesh* mesh, MeshData& meshData, int idx, int order)
	{
		UINT tagentCount = mesh->GetElementTangentCount();
		if (tagentCount != 1)
			return;

		// 탄젠트 data 의 시작 주소
		fbxsdk::FbxGeometryElementTangent* elementTangent = mesh->GetElementTangent();
		UINT vtxIndex = 0;

		if (elementTangent->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
		{
			if (elementTangent->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
				vtxIndex = order;
			else
				vtxIndex = elementTangent->GetIndexArray().GetAt(order);
		}
		else if (elementTangent->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
		{
			if (elementTangent->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
				vtxIndex = idx;
			else
				vtxIndex = elementTangent->GetIndexArray().GetAt(idx);
		}

		fbxsdk::FbxVector4 tangent = elementTangent->GetDirectArray().GetAt(vtxIndex);
		meshData.vertices[idx].tangent.x = (float)tangent.mData[0];
		meshData.vertices[idx].tangent.y = (float)tangent.mData[2];
		meshData.vertices[idx].tangent.z = (float)tangent.mData[1];
	}

	void FbxLoader::getBinormal(fbxsdk::FbxMesh* mesh, MeshData& meshData, int idx, int order)
	{
		UINT biNormalCount = mesh->GetElementBinormalCount();
		if (biNormalCount != 1)
			return;

		// 탄젠트 data 의 시작 주소
		fbxsdk::FbxGeometryElementBinormal* elementBinormal = mesh->GetElementBinormal();
		UINT vtxIndex = 0;

		if (elementBinormal->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
		{
			if (elementBinormal->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
				vtxIndex = order;
			else
				vtxIndex = elementBinormal->GetIndexArray().GetAt(order);
		}
		else if (elementBinormal->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
		{
			if (elementBinormal->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
				vtxIndex = idx;
			else
				vtxIndex = elementBinormal->GetIndexArray().GetAt(idx);
		}

		fbxsdk::FbxVector4 biNormal = elementBinormal->GetDirectArray().GetAt(vtxIndex);
		meshData.vertices[idx].biNormal.x = (float)biNormal.mData[0];
		meshData.vertices[idx].biNormal.y = (float)biNormal.mData[2];
		meshData.vertices[idx].biNormal.z = (float)biNormal.mData[1];
	}

	void FbxLoader::getNormal(fbxsdk::FbxMesh* mesh, MeshData& meshData, int idx, int order)
	{
		UINT normalCount = mesh->GetElementNormalCount();
		if (normalCount != 1)
			return;

		// 탄젠트 data 의 시작 주소
		fbxsdk::FbxGeometryElementNormal* elementNormal = mesh->GetElementNormal();
		UINT vtxIndex = 0;

		if (elementNormal->GetMappingMode() == fbxsdk::FbxGeometryElement::eByPolygonVertex)
		{
			if (elementNormal->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
				vtxIndex = order;
			else
				vtxIndex = elementNormal->GetIndexArray().GetAt(order);
		}
		else if (elementNormal->GetMappingMode() == fbxsdk::FbxGeometryElement::eByControlPoint)
		{
			if (elementNormal->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
				vtxIndex = idx;
			else
				vtxIndex = elementNormal->GetIndexArray().GetAt(idx);
		}

		fbxsdk::FbxVector4 normal = elementNormal->GetDirectArray().GetAt(vtxIndex);
		meshData.vertices[idx].normal.x = (float)normal.mData[0];
		meshData.vertices[idx].normal.y = (float)normal.mData[2];
		meshData.vertices[idx].normal.z = (float)normal.mData[1];
	}

	void FbxLoader::getUV(fbxsdk::FbxMesh* mesh, MeshData& meshData, int idx, int order)
	{
		fbxsdk::FbxGeometryElementUV* elementUV = mesh->GetElementUV();

		UINT iUVIdx = 0;
		if (elementUV->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
			iUVIdx = idx;
		else
			iUVIdx = elementUV->GetIndexArray().GetAt(idx);

		iUVIdx = order;
		fbxsdk::FbxVector2 uv = elementUV->GetDirectArray().GetAt(iUVIdx);
		meshData.vertices[idx].uv.x = (float)uv.mData[0];
		meshData.vertices[idx].uv.y = 1.f - (float)uv.mData[1]; // fbx elementUV 좌표계는 좌하단이 0,0
	}

	Vector4 FbxLoader::GetMaterialColor(fbxsdk::FbxSurfaceMaterial* material, const char* type, const char* typeFactor)
	{
		fbxsdk::FbxDouble3  color;
		fbxsdk::FbxDouble	factor = 0.;

		fbxsdk::FbxProperty mtrlProperty = material->FindProperty(type);
		fbxsdk::FbxProperty factorProperty = material->FindProperty(typeFactor);

		if (mtrlProperty.IsValid() && factorProperty.IsValid())
		{
			color = mtrlProperty.Get<fbxsdk::FbxDouble3>();
			factor = factorProperty.Get<fbxsdk::FbxDouble>();
		}

		Vector4 output = Vector4((float)color.mData[0] * (float)factor
			, (float)color.mData[1] * (float)factor
			, (float)color.mData[2] * (float)factor
			, (float)factor);
		return output;
	}

	std::wstring FbxLoader::GetMaterialTextureName(fbxsdk::FbxSurfaceMaterial* material, const char* type)
	{
		std::string cName = "";
		fbxsdk::FbxProperty textureProperty = material->FindProperty(type);
		if (textureProperty.IsValid())
		{
			UINT count = textureProperty.GetSrcObjectCount();
			if (1 <= count)
			{
				fbxsdk::FbxFileTexture* pFbxTex = textureProperty.GetSrcObject<fbxsdk::FbxFileTexture>(0);
				if (NULL != pFbxTex)
					cName = pFbxTex->GetFileName();
			}
		}

		return std::wstring(cName.begin(), cName.end());
	}

	void FbxLoader::Release()
	{
		mManager->Destroy();
	}

}
