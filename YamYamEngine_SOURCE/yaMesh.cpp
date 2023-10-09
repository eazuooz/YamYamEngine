#include "yaMesh.h"


#include "yaRenderer.h"
#include "yaResources.h"
#include "yaGameObject.h"
#include "yaFbxLoader.h"

namespace ya::graphics
{
	Mesh::Mesh()
		: Resource(eResourceType::Mesh)
		, mMeshes{}
	{
	}

	Mesh::~Mesh()
	{
		for (MeshData* mesh : mMeshes)
		{
			delete mesh;
			mesh = nullptr;
		}
	}

	HRESULT Mesh::Load(const std::wstring& path)
	{
		Assimp::Importer importer;
		std::string cpath(path.begin(), path.end());

		const aiScene* pScene = importer.ReadFile(cpath,
			aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

		Matrix tr;
		ProcessNode(pScene->mRootNode, pScene, tr, path);
		NormalizeVertices();
		for (MeshData* mesh : mMeshes)
			CreateMesh(mesh);

		return S_OK;
	}

	bool Mesh::CreateMesh(std::vector<renderer::Vertex>& vertexes
		, std::vector<UINT>& indices)
	{
		MeshData* meshData = new MeshData();
		CreateVertexBuffer(meshData, vertexes);
		CreateIndexBuffer(meshData, indices, __noop);
		mMeshes.push_back(meshData);

		return true;;
	}

	bool Mesh::CreateMesh(MeshData* meshData)
	{
		CreateVertexBuffer(meshData, meshData->vertices);

		for (size_t i = 0; i < meshData->indices2.size(); i++)
			CreateIndexBuffer(meshData, meshData->indices2[i], i);

		return true;
	}

	bool Mesh::CreateVertexBuffer(MeshData* mesh
		, std::vector<renderer::Vertex>& vertexes)
	{
		mesh->vbDesc.ByteWidth = sizeof(renderer::Vertex) * vertexes.size();
		mesh->vbDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		mesh->vbDesc.Usage = D3D11_USAGE_DEFAULT;
		mesh->vbDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = vertexes.data();
		mesh->vertices = vertexes;

		if (!(GetDevice()->CreateBuffer(&mesh->vbDesc
			, &subData, mesh->vertexBuffer.GetAddressOf())))
			return false;
		
		
		return true;
	}

	bool Mesh::CreateIndexBuffer(MeshData* mesh
		, std::vector<UINT>& indices, UINT index)
	{
		//mIndexCount = Count;
		mesh->ibDesc.ByteWidth = sizeof(UINT) * indices.size();
		mesh->ibDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		mesh->ibDesc.Usage = D3D11_USAGE_DEFAULT;
		mesh->ibDesc.CPUAccessFlags = 0;
		

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = indices.data();

		mesh->indices2[index] = indices;

		if (!(GetDevice()->CreateBuffer(&mesh->ibDesc
			, &subData, mesh->indicesBuffer[index].GetAddressOf())))
			return false;

		return true;
	}

	void Mesh::BindBuffer(MeshData* mesh, UINT index)
	{
		UINT stride = sizeof(renderer::Vertex);
		UINT offset = 0;

		GetDevice()->BindVertexBuffer(0, 1, mesh->vertexBuffer.GetAddressOf(), stride, offset);
		GetDevice()->BindIndexBuffer(mesh->indicesBuffer[index].Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void Mesh::Render()
	{
		for (MeshData* mesh : mMeshes)
		{
			for (size_t i = 0; i < mesh->materials.size(); i++)
			{
				std::shared_ptr<Texture> albedo
					= Resources::Find<Texture>(mesh->materials[i].diffuse);
				std::shared_ptr<graphics::Material> material
					= Resources::Find<Material>(mesh->materials[i].name);

				if (material == nullptr)
				{
					material = Resources::Find<graphics::Material>(L"PhongMaterial");
					material->SetTexture(eTextureType::Albedo, albedo);
				}
				material->Bind();

				BindBuffer(mesh, i);
				GetDevice()->DrawIndexed(mesh->indices2[i].size(), 0, 0);
			}
		}
	}

	void Mesh::RenderInstanced(UINT startIndexLocation)
	{
		//for (MeshData* mesh : mMeshes)
		//{
		//	BindBuffer(mesh);
		//	GetDevice()->DrawIndexedInstanced(mesh->indices.size()
		//		, startIndexLocation, 0, 0, 0);
		//}
	}

	void Mesh::ProcessNode(aiNode* node, const aiScene* scene, Matrix tr, const std::wstring& path) 
	{
		Matrix m;
		ai_real* temp = &node->mTransformation.a1;
		float* mTemp = &m._11;
		for (int t = 0; t < 16; t++) 
		{
			mTemp[t] = float(temp[t]);
		}
		m = m.Transpose() * tr;

		for (UINT i = 0; i < node->mNumMeshes; i++) 
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			MeshData* newMesh = ProcessMesh(mesh, scene, path);

			for (auto& v : newMesh->vertices) 
			{
				v.pos = Vector3::Transform(v.pos, m);
			}

			mMeshes.push_back(newMesh);
		}

		for (UINT i = 0; i < node->mNumChildren; i++) 
			ProcessNode(node->mChildren[i], scene, m, path);
	}

	MeshData* Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::wstring& path) 
	{
		// Data to fill
		std::vector<renderer::Vertex> vertices;
		std::vector<uint32_t> indices;

		// Walk through each of the meshData's vertices
		for (UINT i = 0; i < mesh->mNumVertices; i++) 
		{
			renderer::Vertex vertex;

			vertex.pos.x = mesh->mVertices[i].x;
			vertex.pos.y = mesh->mVertices[i].y;
			vertex.pos.z = mesh->mVertices[i].z;

			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
			vertex.normal.Normalize();

			if (mesh->mTextureCoords[0]) 
			{
				vertex.uv.x = (float)mesh->mTextureCoords[0][i].x;
				vertex.uv.y = (float)mesh->mTextureCoords[0][i].y;
			}

			vertices.push_back(vertex);
		}

		for (UINT i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (UINT j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		MeshData* newMesh = new MeshData();
		newMesh->vertices = vertices;
		newMesh->indices2.resize(1);
		newMesh->indices2[0] = indices;

		// http://assimp.sourceforge.net/lib_html/materials.html
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
				aiString filepath;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &filepath);

				std::filesystem::path fs(path);
				fs.remove_filename();

				std::string fullPath = fs.string();
				std::string textureName 
					= std::string(std::filesystem::path(filepath.C_Str()).filename().string());
				fullPath += textureName;

				MeshData::MaterialData material = {};
				material.diffuse = std::wstring(textureName.begin(), textureName.end());
				newMesh->materials.push_back(material);

				std::wstring key(textureName.begin(), textureName.end());
				std::wstring path(fullPath.begin(), fullPath.end());

				Resources::Load<Texture>(key, path);
			}
		}

		return newMesh;
	}

	void Mesh::NormalizeVertices()
	{
		Vector3 vmin(1000, 1000, 1000);
		Vector3 vmax(-1000, -1000, -1000);
		for (auto& mesh : mMeshes)
		{
			for (auto& v : mesh->vertices)
			{
				vmin.x = XMMin(vmin.x, v.pos.x);
				vmin.y = XMMin(vmin.y, v.pos.y);
				vmin.z = XMMin(vmin.z, v.pos.z);
				vmax.x = XMMax(vmax.x, v.pos.x);
				vmax.y = XMMax(vmax.y, v.pos.y);
				vmax.z = XMMax(vmax.z, v.pos.z);
			}
		}

		float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
		float dl = XMMax(XMMax(dx, dy), dz);
		float cx = (vmax.x + vmin.x) * 0.5f, cy = (vmax.y + vmin.y) * 0.5f,
			cz = (vmax.z + vmin.z) * 0.5f;

		for (auto& mesh : mMeshes)
		{
			for (auto& v : mesh->vertices)
			{
				v.pos.x = (v.pos.x - cx) / dl;
				v.pos.y = (v.pos.y - cy) / dl;
				v.pos.z = (v.pos.z - cz) / dl;
			}
		}
	}
	void Mesh::LoadFromFbx(const std::wstring& path)
	{
		FbxLoader loader;
		loader.Initialize();
		loader.Load(path);

		std::vector<MeshData>& meshDatas = loader.GetMeshDatas();
		for (MeshData& data : meshDatas)
		{
			MeshData* meshData = new MeshData();
			meshData->name = data.name;
			meshData->vertexBuffer = data.vertexBuffer;
			meshData->indicesBuffer = data.indicesBuffer;
			meshData->vertices = data.vertices;
			meshData->indices2 = data.indices2;
			meshData->vbDesc = data.vbDesc;
			meshData->ibDesc = data.ibDesc;
			meshData->materials = data.materials;

			mMeshes.push_back(meshData);
		}
	}
}
