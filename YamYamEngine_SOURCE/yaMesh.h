#pragma once
#include "yaResource.h"
#include "yaRenderer.h"
#include "yaMaterial.h"
#include "yaMeshData.h"

#include "..\\External\\Include\\Assimp\\Importer.hpp"
#include "..\\External\\Include\\Assimp\\postprocess.h"
#include "..\\External\\Include\\Assimp\\scene.h"



#ifdef _DEBUG
#pragma comment(lib, "..\\External\\Library\\Assimp\\Debug\\assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "..\\External\\Library\\Assimp\\Release\\assimp-vc143-mt.lib")
#endif

namespace ya
{
	class Mesh : public Resource
	{
	public:
		Mesh();
		~Mesh();

		virtual HRESULT Load(const std::wstring& path) override;

		bool  CreateMesh(std::vector<renderer::Vertex>& vertexes
			, std::vector<UINT>& indices);
		bool CreateMesh(MeshData* mesh);
		bool CreateVertexBuffer(MeshData* mesh, std::vector<renderer::Vertex>& vertexes);
		bool CreateIndexBuffer(MeshData* mesh, std::vector<UINT>& indices, UINT index);
		void BindBuffer(MeshData* mesh, UINT index);
		void Render();
		void RenderInstanced(UINT count);

		void ProcessNode(aiNode* node, const aiScene* scene,
			ya::math::Matrix tr, const std::wstring& path);

		MeshData* ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::wstring& path);
		void NormalizeVertices();
		void LoadFromFbx(const std::wstring& path, class Animator* animator = nullptr);

		std::vector<MeshData*> GetMeshDatas() { return mMeshes; }
		void SetMaterialName(const std::wstring& name) { mMeshes[0]->materials[0].name = name; }

	private:
		std::vector<MeshData*> mMeshes;
		//std::vector<MeshData::Bone
	};
}
