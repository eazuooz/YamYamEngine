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
#pragma comment(lib, "..\\External\\Library\\Assimp\\Release\\assimp-vc143-mtd.lib")
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
			std::wstring textureFilename;

			D3D11_BUFFER_DESC vbDesc;
			D3D11_BUFFER_DESC ibDesc;
		};

		Mesh();
		~Mesh();

		virtual HRESULT Load(const std::wstring& path) override;

		bool CreateVertexBuffer(void* data, UINT Count);
		bool CreateIndexBuffer(void* data, UINT Count);
		void BindBuffer();
		void Render();
		void RenderInstanced(UINT count);

	private:
		std::vector<MeshData> mMeshes;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		D3D11_BUFFER_DESC mVBDesc;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		D3D11_BUFFER_DESC mIBDesc;
		UINT mIndexCount;
	};
}
