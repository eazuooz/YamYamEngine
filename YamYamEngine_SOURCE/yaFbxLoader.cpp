#include "yaFbxLoader.h"
#include "yaResources.h"
#include "yaTexture.h"
#include "yaMaterial.h"

#include <filesystem>

namespace ya
{
	FbxLoader::FbxLoader()
	{
	}
	FbxLoader::~FbxLoader()
	{
		

		Release();
	}

	void FbxLoader::Initialize()
	{
		mManager = FbxManager::Create();
		fbxsdk::FbxIOSettings* ioSettings
			= fbxsdk::FbxIOSettings::Create(mManager, IOSROOT);
		mManager->SetIOSettings(ioSettings);
	}

	bool FbxLoader::Load(const std::wstring& path)
	{
		mScene = fbxsdk::FbxScene::Create(mManager, "");
		mImporter = fbxsdk::FbxImporter::Create(mManager, "");

		std::filesystem::path currentDirectory = std::filesystem::current_path();
		currentDirectory = currentDirectory.parent_path();
		currentDirectory += L"\\Resources\\fbx\\";
		currentDirectory += path;

		std::string cpath(currentDirectory.string());
		if (!mImporter->Initialize(cpath.c_str(), -1, mManager->GetIOSettings()))
 			return false;

		mImporter->Import(mScene);
		mScene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::Max);

		fbxsdk::FbxNode* rootNode = mScene->GetRootNode();
		mMeshDatas.clear();
		
		MeshData* meshData = new MeshData();
		mMeshDatas.push_back(meshData);

		//Animation
		loadBoneData(rootNode, 0, 0, -1, meshData);
		loadAnimationClips(mScene);

		triangulate(rootNode);

		loadMeshData(rootNode, meshData);
		loadTextures();

		mImporter->Destroy();
		mScene->Destroy();
		
		return true;
	}

	bool FbxLoader::CreateMesh()
	{
		// Vertex
		for (MeshData* meshData : mMeshDatas)
		{
			UINT vtxCount = meshData->vertices.size();
			D3D11_BUFFER_DESC vtxDesc = {};

			vtxDesc.ByteWidth = sizeof(renderer::Vertex) * meshData->vertices.size();
			vtxDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
			vtxDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

			renderer::Vertex* pVtxMem = new renderer::Vertex[meshData->vertices.size()];
			for (size_t i = 0; i < meshData->vertices.size(); i++)
			{
				renderer::Vertex vtx = meshData->vertices[i];

				pVtxMem[i].pos = vtx.pos;
				pVtxMem[i].uv = vtx.uv;
				pVtxMem[i].color = vtx.color;
				pVtxMem[i].normal = vtx.normal;
				pVtxMem[i].biNormal = vtx.biNormal;
				pVtxMem[i].tangent = vtx.tangent;
			}
			D3D11_SUBRESOURCE_DATA sub = {};
			sub.pSysMem = pVtxMem;

			if (FAILED(graphics::GetDevice()->CreateBuffer(&vtxDesc, &sub, meshData->vertexBuffer.GetAddressOf())))
				return false;

			delete[] pVtxMem;
			
			//Index
			D3D11_BUFFER_DESC idxDesc = {};
			UINT idicesSize = meshData->indices.size();
			meshData->indicesBuffer.resize(idicesSize);
			for (size_t j = 0; j < idicesSize; j++)
			{
				idxDesc.ByteWidth = sizeof(UINT) * meshData->indices[j].size();
				idxDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
				idxDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

				void* pIdxMem = new UINT[meshData->indices[j].size()];
				memcpy(pIdxMem, meshData->indices[j].data(), idxDesc.ByteWidth);
				sub.pSysMem = pIdxMem;

				if (FAILED(graphics::GetDevice()->CreateBuffer(&idxDesc, &sub, meshData->indicesBuffer[j].GetAddressOf())))
					return false;

				delete[] pIdxMem;
			}
		}
		
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

	void FbxLoader::loadMeshData(fbxsdk::FbxNode* node, MeshData* meshData)
	{
		
		fbxsdk::FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
		if (nodeAttribute && fbxsdk::FbxNodeAttribute::eMesh == nodeAttribute->GetAttributeType())
		{
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


		int childCount = node->GetChildCount();
		for (int i = 0; i < childCount; ++i)
			loadMeshData(node->GetChild(i), meshData);
	}

	void FbxLoader::loadMesh(fbxsdk::FbxMesh* mesh, MeshData* meshData)
	{
		UINT vtxCount = mesh->GetPolygonSize(0);
	
		if (vtxCount != 3)
			return;

		std::string cName = mesh->GetName();
		meshData->name = std::wstring(cName.begin(), cName.end());

		loadVertexData(mesh, meshData);

		//load animation data
		loadAnimationVertexData(mesh, meshData);

	}

	void FbxLoader::loadVertexData(fbxsdk::FbxMesh* mesh, MeshData* meshData)
	{
		//vertex
		getPosition(mesh, meshData);

		UINT materialCount = mesh->GetNode()->GetMaterialCount();
		meshData->indices.resize(materialCount);
		//meshData->mBoneWeights.resize(materialCount);

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
			meshData->indices[subset].push_back(indices[0]);
			meshData->indices[subset].push_back(indices[2]);
			meshData->indices[subset].push_back(indices[1]);
		}
	}

	void FbxLoader::loadAnimationVertexData(fbxsdk::FbxMesh* mesh, MeshData* meshData)
	{
		UINT skiningCount = mesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);
		if (skiningCount == 0)
			return;

		meshData->bAnimation = true;
		for (size_t i = 0; i < skiningCount; i++)
		{
			fbxsdk::FbxSkin* skin = (fbxsdk::FbxSkin*)mesh->GetDeformer(i, fbxsdk::FbxDeformer::eSkin);
			fbxsdk::FbxSkin::EType type = skin->GetSkinningType();

			if (fbxsdk::FbxSkin::eRigid == type || fbxsdk::FbxSkin::eLinear == type)
			{
				UINT clusterCount = skin->GetClusterCount();
				for (size_t j = 0; j < clusterCount; j++)
				{
					fbxsdk::FbxCluster* cluster = skin->GetCluster(j);

					if (!cluster->GetLink())
						continue;

					int boneIdx = getBoneIndex(cluster->GetLink()->GetName(), meshData);
					if (-1 == boneIdx)
						assert(NULL);

					fbxsdk::FbxAMatrix globalTransform = GetTransformMatrix(mesh->GetNode());

					LoadWeightsAndIndices(cluster, boneIdx, meshData);
					LoadKeyframeTransform(mesh->GetNode(), cluster, globalTransform, boneIdx, meshData);
					LoadOffsetMatrix(cluster, globalTransform, boneIdx, meshData);
				}

			}
		}

		loadWeightsAndIndices(mesh, meshData);
	}


	void FbxLoader::loadMaterial(fbxsdk::FbxSurfaceMaterial* material, MeshData* meshData)
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

		meshData->materials.push_back(materialInfo);
	}

	void FbxLoader::loadTextures()
	{
		// 텍스처 로드
		std::filesystem::path parentPath = std::filesystem::current_path().parent_path();
		std::filesystem::path path_fbx_texture = parentPath.wstring() + L"\\Resources\\fbx\\Texture\\";

		std::filesystem::path path_origin;
		std::filesystem::path path_filename;
		std::filesystem::path path_dest;

		for (MeshData* data : mMeshDatas)
		{
			for (MeshData::MaterialData& matData : data->materials)
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

	
	void FbxLoader::loadAnimationClips(fbxsdk::FbxScene* scene)
	{
		fbxsdk::FbxArray<fbxsdk::FbxString*> animationNames = {};
		scene->FillAnimStackNameArray(animationNames);

		int animationCount = animationNames.GetCount();
		for (int i = 0; i < animationCount; ++i)
		{
			fbxsdk::FbxAnimStack* animStack
				= mScene->FindMember<fbxsdk::FbxAnimStack>(animationNames[i]->Buffer());

			if (!animStack)
				continue;

			AnimationClip* animCip = new AnimationClip();
			std::string strClipName = animStack->GetName();
			animCip->name = std::wstring(strClipName.begin(), strClipName.end());
			fbxsdk::FbxTakeInfo* takeInfo = mScene->GetTakeInfo(animStack->GetName());
			animCip->startTime = takeInfo->mLocalTimeSpan.GetStart();
			animCip->endTime = takeInfo->mLocalTimeSpan.GetStop();
			animCip->mode = mScene->GetGlobalSettings().GetTimeMode();
			animCip->timeLength = animCip->endTime.GetFrameCount(animCip->mode)
				- animCip->startTime.GetFrameCount(animCip->mode);

			mAnimationClips.push_back(animCip);
		}
		
		for (size_t i = 0; i < animationCount; i++)
		{
			delete animationNames[i];
		}
	}

	void FbxLoader::loadBoneData(fbxsdk::FbxNode* node, int depth, int idx, int parentIdx, MeshData* meshData)
	{
		fbxsdk::FbxNodeAttribute* pAttr = node->GetNodeAttribute();

		if (pAttr && pAttr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
		{
			MeshData::Bone pBone = {};

			std::string strBoneName = node->GetName();

			pBone.name = std::wstring(strBoneName.begin(), strBoneName.end());
			pBone.depth = depth++;
			pBone.parentIdx = parentIdx;

			meshData->bones.push_back(pBone);
		}

		int iChildCount = node->GetChildCount();
		for (int i = 0; i < iChildCount; ++i)
		{
			loadBoneData(node->GetChild(i), depth, (int)meshData->bones.size(), idx, meshData);
		}
	}


	void FbxLoader::CreateMaterial()
	{
		// 메테리얼 로드
		std::wstring key;
		std::wstring path;

		for (MeshData* data : mMeshDatas)
		{
			for (MeshData::MaterialData& materialData : data->materials)
			{
				std::shared_ptr<graphics::Material> material = std::make_shared<graphics::Material>();

				// Material 이름짓기
				key = materialData.name;
				if (key.empty())
					key = std::filesystem::path(materialData.diffuse).stem();

				path = L"material\\";
				path += key;

				// 상대경로가 곧 이름(확장자명은 제외)
				materialData.name = key;
				material->SetKey(key);
				material->SetPath(path + L".mtrl");

				std::shared_ptr<graphics::Shader> phong = Resources::Find<graphics::Shader>(L"PhongShader");
				material->SetShader(phong);

				std::wstring strTexKey = materialData.diffuse;
				std::shared_ptr<graphics::Texture> pTex = Resources::Find<graphics::Texture>(strTexKey);
				if (NULL != pTex)
					material->SetTexture(graphics::eTextureType::Albedo, pTex);

				strTexKey = materialData.normal;
				pTex = Resources::Find<graphics::Texture>(strTexKey);
				if (NULL != pTex)
					material->SetTexture(graphics::eTextureType::Normal, pTex);

				strTexKey = materialData.specular;
				pTex = Resources::Find<graphics::Texture>(strTexKey);
				if (NULL != pTex)
					material->SetTexture(graphics::eTextureType::Specular, pTex);

				material->SetColor(materialData.diffuseColor, materialData.specularColor, materialData.ambientColor);

				material->SetRenderingMode(graphics::eRenderingMode::Opaque);
				Resources::Insert<graphics::Material>(material->GetKey(), material);
			}
		}

		int a = 0;
	}

	void FbxLoader::getPosition(fbxsdk::FbxMesh* mesh, MeshData* meshData)
	{
		UINT vtxCount = mesh->GetControlPointsCount();
		meshData->vertices.resize(vtxCount);
		mBoneWeights.resize(vtxCount);

		fbxsdk::FbxVector4* positions = mesh->GetControlPoints();
		for (size_t i = 0; i < vtxCount; i++)
		{
			meshData->vertices[i].pos.x = static_cast<float>(positions[i].mData[0]);
			meshData->vertices[i].pos.y = static_cast<float>(positions[i].mData[2]);
			meshData->vertices[i].pos.z = static_cast<float>(positions[i].mData[1]);
		}
	}

	void FbxLoader::getTangent(fbxsdk::FbxMesh* mesh, MeshData* meshData, int idx, int order)
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
		meshData->vertices[idx].tangent.x = (float)tangent.mData[0];
		meshData->vertices[idx].tangent.y = (float)tangent.mData[2];
		meshData->vertices[idx].tangent.z = (float)tangent.mData[1];
	}

	void FbxLoader::getBinormal(fbxsdk::FbxMesh* mesh, MeshData* meshData, int idx, int order)
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
		meshData->vertices[idx].biNormal.x = (float)biNormal.mData[0];
		meshData->vertices[idx].biNormal.y = (float)biNormal.mData[2];
		meshData->vertices[idx].biNormal.z = (float)biNormal.mData[1];
	}

	void FbxLoader::getNormal(fbxsdk::FbxMesh* mesh, MeshData* meshData, int idx, int order)
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
		meshData->vertices[idx].normal.x = (float)normal.mData[0];
		meshData->vertices[idx].normal.y = (float)normal.mData[2];
		meshData->vertices[idx].normal.z = (float)normal.mData[1];
	}

	void FbxLoader::getUV(fbxsdk::FbxMesh* mesh, MeshData* meshData, int idx, int order)
	{
		fbxsdk::FbxGeometryElementUV* elementUV = mesh->GetElementUV();

		UINT iUVIdx = 0;
		if (elementUV->GetReferenceMode() == fbxsdk::FbxGeometryElement::eDirect)
			iUVIdx = idx;
		else
			iUVIdx = elementUV->GetIndexArray().GetAt(idx);

		iUVIdx = order;
		fbxsdk::FbxVector2 uv = elementUV->GetDirectArray().GetAt(iUVIdx);
		meshData->vertices[idx].uv.x = (float)uv.mData[0];
		meshData->vertices[idx].uv.y = 1.f - (float)uv.mData[1]; // fbx elementUV 좌표계는 좌하단이 0,0
	}

	Matrix FbxLoader::getMatrixFromFbxMatrix(fbxsdk::FbxAMatrix& matrix)
	{
		Matrix mat;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				mat.m[i][j] = (float)matrix.Get(i, j);
			}
		}
		return mat;
	}

	int FbxLoader::getBoneIndex(std::string name, MeshData* meshData)
	{
		for (size_t i = 0; i < meshData->bones.size(); i++)
		{
			if (meshData->bones[i].name == std::wstring(name.begin(), name.end()))
				return i;
		}

		return -1;
	}

	fbxsdk::FbxAMatrix FbxLoader::GetTransformMatrix(fbxsdk::FbxNode* node)
	{
		const fbxsdk::FbxVector4 t = node->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
		const fbxsdk::FbxVector4 r = node->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
		const fbxsdk::FbxVector4 s = node->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);

		return fbxsdk::FbxAMatrix(t, r, s);
	}

	void FbxLoader::LoadWeightsAndIndices(fbxsdk::FbxCluster* cluster, int boneIndex0, MeshData* meshData)
	{
		int indicesCount = cluster->GetControlPointIndicesCount();

		for (size_t i = 0; i < indicesCount; i++)
		{
			MeshData::BoneWeight bw = {};
			bw.boneIndex0 = boneIndex0;

			double weight = cluster->GetControlPointWeights()[i];
			bw.weight0 = weight;

			int vtxIdx = cluster->GetControlPointIndices()[i];
			mBoneWeights[vtxIdx].push_back(bw);
		}
	}


	void ConvertCoordinate(fbxsdk::FbxAMatrix& matrix)
	{
		fbxsdk::FbxVector4 v0 = { 1, 0, 0, 0 };
		fbxsdk::FbxVector4 v1 = { 0, 0, 1, 0 };
		fbxsdk::FbxVector4 v2 = { 0, 1, 0, 0 };
		fbxsdk::FbxVector4 v3 = { 0, 0, 0, 1 };

		fbxsdk::FbxAMatrix convert;
		convert[0] = v0;
		convert[1] = v1;
		convert[2] = v2;
		convert[3] = v3;

		matrix = convert * matrix * convert;
	}

	void FbxLoader::LoadOffsetMatrix(fbxsdk::FbxCluster* cluster, const fbxsdk::FbxAMatrix& globalTransform, int boneIdx, MeshData* meshData)
	{
		fbxsdk::FbxAMatrix clusterTransform;
		fbxsdk::FbxAMatrix globalMatrix;

		cluster->GetTransformMatrix(clusterTransform);
		cluster->GetTransformLinkMatrix(globalMatrix);

		fbxsdk::FbxAMatrix offset;
		offset = globalMatrix.Inverse() * clusterTransform * globalTransform;
		ConvertCoordinate(offset); //reflect * matOffset * reflect;
				
		meshData->bones[boneIdx].offset = getMatrixFromFbxMatrix(offset);
	}

	void FbxLoader::loadWeightsAndIndices(fbxsdk::FbxMesh* mesh, MeshData* meshData)
	{
		std::vector<std::vector<MeshData::BoneWeight>>::iterator iter = mBoneWeights.begin();

		//std::for_each(mBoneWeights.begin(), mBoneWeights.end(),
		//	[](std::vector<MeshData::BoneWeight>& boneWeight)
		//	{
		//		if (boneWeight.size() > 1)
		//		{
		//			sort(boneWeight.begin(), boneWeight.end()
		//				, [](const MeshData::BoneWeight& left, const MeshData::BoneWeight& right)
		//				{
		//					return left.weight0 > right.weight0;
		//				});
		//		}
		//		//...
		//	});

		int iVtxIdx = 0;
		for (iter; iter != mBoneWeights.end(); ++iter, ++iVtxIdx)
		{
			if ((*iter).size() > 1)
			{
				// 가중치 값 순으로 내림차순 정렬
				sort((*iter).begin(), (*iter).end()
					, [](const MeshData::BoneWeight& left, const MeshData::BoneWeight& right)
					{
						return left.weight0 > right.weight0;
					}
				);

				double dWeight = 0.f;
				for (UINT i = 0; i < (*iter).size(); ++i)
				{
					dWeight += (*iter)[i].weight0;
				}

				// 가중치의 합이 1이 넘어가면 처음부분에 더해준다.
				double revision = 0.f;
				if (dWeight > 1.0)
				{
					revision = 1.0 - dWeight;
					(*iter)[0].weight0 += revision;
				}

				if ((*iter).size() >= 4)
				{
					(*iter).erase((*iter).begin() + 4, (*iter).end());
				}
			}

			// 정점 정보로 변환, 
			float fWeights[4] = {};
			float fIndices[4] = {};

			for (UINT i = 0; i < (*iter).size(); ++i)
			{
				fWeights[i] = (float)(*iter)[i].weight0;
				fIndices[i] = (float)(*iter)[i].boneIndex0;
			}

			meshData->vertices[iVtxIdx].weight0 = fWeights[0];
			meshData->vertices[iVtxIdx].weight1 = fWeights[1];
			meshData->vertices[iVtxIdx].weight2 = fWeights[2];
			meshData->vertices[iVtxIdx].weight3 = fWeights[3];

			meshData->vertices[iVtxIdx].boneIndex0 = fIndices[0];
			meshData->vertices[iVtxIdx].boneIndex1 = fIndices[1];
			meshData->vertices[iVtxIdx].boneIndex2 = fIndices[2];
			meshData->vertices[iVtxIdx].boneIndex3 = fIndices[3];
		}
	}

	void FbxLoader::LoadKeyframeTransform(fbxsdk::FbxNode* parentNode, fbxsdk::FbxCluster* cluster
		, fbxsdk::FbxAMatrix& globalTransform, int boneIdx, MeshData* meshData)
	{
		if (mAnimationClips.empty())
			return;

		meshData->bones[boneIdx].globalTransform = getMatrixFromFbxMatrix(globalTransform);

		fbxsdk::FbxTime::EMode eTimeMode = mScene->GetGlobalSettings().GetTimeMode();

		fbxsdk::FbxLongLong startFrame = mAnimationClips[0]->startTime.GetFrameCount(eTimeMode);
		fbxsdk::FbxLongLong endFrame = mAnimationClips[0]->endTime.GetFrameCount(eTimeMode);

		for (fbxsdk::FbxLongLong i = startFrame; i < endFrame; ++i)
		{
			MeshData::Bone::KeyFrame frame = {};
			fbxsdk::FbxTime time = 0;

			time.SetFrame(i, eTimeMode);

			fbxsdk::FbxAMatrix toMeshTransform = parentNode->EvaluateGlobalTransform(time) * globalTransform;
			fbxsdk::FbxAMatrix toRootTransform = cluster->GetLink()->EvaluateGlobalTransform(time);
			fbxsdk::FbxAMatrix transform = toMeshTransform.Inverse() * toRootTransform;
			
			ConvertCoordinate(transform);
			frame.time = time.GetSecondDouble();
			frame.transform = getMatrixFromFbxMatrix(transform);;

			fbxsdk::FbxVector4 translate = transform.GetT();
			fbxsdk::FbxVector4 scale = transform.GetS();
			fbxsdk::FbxQuaternion quaternion = transform.GetQ();
			
			frame.translate = Vector3(translate.mData[0], translate.mData[1] , translate.mData[2]);
			frame.scale = Vector3(scale.mData[0], scale.mData[1], scale.mData[2]);
			frame.quarternion = Vector4(quaternion.mData[0], quaternion.mData[1], quaternion.mData[2], quaternion.mData[3]);

			meshData->bones[boneIdx].keyFrames.push_back(frame);
		}
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

		for (auto ani : mAnimationClips)
		{
			delete ani;
			ani = nullptr;
		}
	}

}
