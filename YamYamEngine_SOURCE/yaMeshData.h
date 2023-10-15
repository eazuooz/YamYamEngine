#pragma once

#include "yaGraphics.h"
#include "yaRenderer.h"


namespace ya
{
	struct MeshData
	{
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

		struct BoneWeight
		{
			int boneIndex0;
			int boneIndex1;
			int boneIndex2;
			int boneIndex3;

			double weight0;
			double weight1;
			double weight2;
			double weight3;
		};

		struct Bone
		{
			struct KeyFrame
			{
				Matrix toRootTransform;
				double time;
			};

			std::wstring name;
			int depth;		
			int parentIdx; 
			Matrix offset; 
			Matrix globalTransform;

			std::vector<KeyFrame> keyFrames;
		};

		typedef std::vector<UINT> IndexBuffer;

		MeshData()
			: vbDesc{}
			, ibDesc{}
			, vertexBuffer(nullptr)
			, vertices{}
			, materials{}
			, name(L"")
			, bAnimation(false)
			

		{
			indicesBuffer.resize(1);
			indices.resize(1);
		}
		~MeshData()
		{

		}

		std::wstring name;
		std::vector<renderer::Vertex> vertices;
		std::vector<IndexBuffer> indices;

		std::vector<MaterialData> materials;

		bool bAnimation;
		std::vector<MeshData::Bone> bones;

		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		std::vector <Microsoft::WRL::ComPtr<ID3D11Buffer>> indicesBuffer;
		D3D11_BUFFER_DESC vbDesc;
		D3D11_BUFFER_DESC ibDesc;
	};
}
