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
			std::wstring diffuse;
			std::wstring normal;
			std::wstring specular;
			std::wstring emessive;

			Vector4 diffuseColor;
			Vector4 specularColor;
			Vector4 ambientColor;
			Vector4 emessiveColor;
		};

		MeshData()
			: vbDesc{}
			, ibDesc{}
			, vertexBuffer(nullptr)
			, vertices{}
			, materials{}
			, name(L"")

		{
			indicesBuffer.resize(1);
			indices2.resize(1);
		}
		
		~MeshData()
		{
			
		}

		std::wstring name;

		// vb, ib
		std::vector<renderer::Vertex> vertices;
		std::vector<std::vector<UINT>> indices2;

		// material
		std::vector<MaterialData> materials;

		// animation
		//bool bAnimation;
		//std::vector<std::vector<BoneWeight>> boneWeights;
		//std::vector<Vector4> skiningWeights;
		//std::vector<Vector4> skiningIndices;

		// gpu		
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		std::vector <Microsoft::WRL::ComPtr<ID3D11Buffer>> indicesBuffer;
		D3D11_BUFFER_DESC vbDesc;
		D3D11_BUFFER_DESC ibDesc;
	};
}
