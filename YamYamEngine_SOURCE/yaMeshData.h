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
			double weight0;
		};

		struct Bone
		{
			struct KeyFrame
			{
				Matrix rootTransform;
				Vector4 translate;
				Vector4 scale;
				Vector4 quarternion;
				double time;
				int frame;
			};

			std::wstring name;
			int depth;		
			int parentIdx; 
			Matrix offset; 
			//Matrix globalTransform;

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
			delete rootMatrices;
			delete offsetMatrices;
		}
		void CreateStructedBuffers()
		{
			struct KeyData
			{
				Vector4 translate;
				Vector4 scale;
				Vector4 quarternion;
			};

			std::vector<Matrix> offsets;
			std::vector<KeyData> keyframes;

			UINT maxFrameCount = 0;
			std::vector<MeshData::Bone>::iterator iter 
			= std::max_element(bones.begin(), bones.end(),
				[](MeshData::Bone& a, MeshData::Bone& b)
				{
					int aSize = a.keyFrames.size();
					int bSize = b.keyFrames.size();
					return aSize < bSize;
				});
			maxFrameCount = iter->keyFrames.size();
			keyframes.resize(bones.size() * maxFrameCount);

			for (size_t i = 0; i < bones.size(); ++i)
			{
				offsets.push_back(bones[i].offset);

				for (size_t j = 0; j < bones[i].keyFrames.size(); ++j)
				{
					UINT idx = (UINT)bones.size() * j + i;
					keyframes[idx].translate = bones[i].keyFrames[j].translate;
					keyframes[idx].quarternion = bones[i].keyFrames[j].quarternion;
					keyframes[idx].scale = bones[i].keyFrames[j].scale;
				}
			}

			rootMatrices = new graphics::StructedBuffer();
			offsetMatrices = new graphics::StructedBuffer();

			offsetMatrices->Create(sizeof(Matrix), (UINT)offsets.size(), graphics::eViewType::SRV, offsets.data(), false);
			rootMatrices->Create(sizeof(KeyData), bones.size() * maxFrameCount , graphics::eViewType::SRV, keyframes.data(), false);
		}

		std::wstring name;
		std::vector<renderer::Vertex> vertices;
		std::vector<IndexBuffer> indices;

		std::vector<MaterialData> materials;

		bool bAnimation;
		std::vector<MeshData::Bone> bones;
		graphics::StructedBuffer* rootMatrices; 
		graphics::StructedBuffer* offsetMatrices; 

		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		std::vector <Microsoft::WRL::ComPtr<ID3D11Buffer>> indicesBuffer;
		D3D11_BUFFER_DESC vbDesc;
		D3D11_BUFFER_DESC ibDesc;
	};
}
