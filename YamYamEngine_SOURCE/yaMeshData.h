#pragma once
#include "yaGraphics.h"
#include "yaRenderer.h"
//#include "yaResource.h"

namespace ya
{
	struct MeshData //: public Resource
	{
		struct BoneWeight
		{
			UINT index;
			double weight;
		};

		struct MaterialData
		{
			std::wstring name;
			std::string diffuse;
			std::wstring normal;
			std::wstring specular;
			std::wstring emessive;

			Vector4 diffuseColor;
			Vector4 specularColor;
			Vector4 ambientColor;
			Vector4 emessiveColor;
		};
		
		std::wstring name;

		// vb, ib
		std::vector<renderer::Vertex> vertices;
		std::vector<UINT> indices;
		std::string diffuse;

		//std::vector<std::vector<UINT>> indices;

		// material
		std::vector<MaterialData> materials;

		// animation
		bool bAnimation;
		std::vector<std::vector<BoneWeight>> boneWeights;
		std::vector<Vector4> skiningWeights;
		std::vector<Vector4> skiningIndices;

		// gpu		
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
		D3D11_BUFFER_DESC vbDesc;
		D3D11_BUFFER_DESC ibDesc;

		//void Resize(UINT size)
		//{
		//	vertices.resize(size);
		//	indices.resize(size);
		//	weightAndIndices.resize(size);

		//	skiningWeights.resize(size);
		//	skiningIndices.resize(size);
		//}
	};
}
