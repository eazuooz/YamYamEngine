#pragma once
#include "yaResource.h"
#include "yaGraphics.h"
#include "yaRenderer.h"

#include "..\\External\\Include\\Assimp\\Importer.hpp"
#include "..\\External\\Include\\Assimp\\postprocess.h"
#include "..\\External\\Include\\Assimp\\scene.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\Library\\Assimp\\Debug\\assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "..\\External\\Library\\Assimp\\Release\\assimp-vc143-mt.lib")
#endif

namespace ya::graphics
{
	class Mesh : public Resource
	{
	public:
		struct MeshData
		{
			std::vector<renderer::Vertex> vertices;
			std::vector<UINT> indices;
			std::string diffuse;

			Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
			Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
			D3D11_BUFFER_DESC vbDesc;
			D3D11_BUFFER_DESC ibDesc;
		};

		Mesh();
		~Mesh();

		virtual HRESULT Load(const std::wstring& path) override;

		bool  CreateMesh(std::vector<renderer::Vertex>& vertexes
			, std::vector<UINT>& indices);
		bool CreateMesh(MeshData* mesh);
		bool CreateVertexBuffer(MeshData* mesh, std::vector<renderer::Vertex>& vertexes);
		bool CreateIndexBuffer(MeshData* mesh, std::vector<UINT>& indices);
		void BindBuffer(MeshData* mesh);
		void Render();
		void RenderInstanced(UINT count);

		void ProcessNode(aiNode* node, const aiScene* scene,
			ya::math::Matrix tr, const std::wstring& path);

		MeshData* ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::wstring& path);
		void NormalizeVertices();

	private:
		std::vector<MeshData*> mMeshes;
	};
}
