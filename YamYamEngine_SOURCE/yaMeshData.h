#pragma once

#include "yaGraphics.h"
#include "yaRenderer.h"


//#include <fbxsdk.h>
//#include <fbxsdk/scene/geometry/fbxnurbs.h >
//#include "..\\External\\Include\\Fbx\\fbxsdk.h"
//#include <fbxsdk.h>



namespace ya
{
	struct MeshData //: public Resource
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
			int boneIdx;
			double weight;
		};

		struct Bone
		{
			struct KeyFrame
			{
				Matrix transform;
				double time;
			};

			std::wstring name;
			int depth;		//°èÃþ±¸Á¶ ±íÀÌ
			int parentIdx; //ºÎ¸ðÀÇ º» ÀÎµ¦½º
			Matrix offsetMatrix; // ³ª --> ºÎ¸ð --> »Ñ¸® --> local
			Matrix boneMatrix;

			std::vector<KeyFrame> keyFrames;
		};

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
			indices2.resize(1);
		}
		~MeshData()
		{

		}

		std::wstring name;
		std::vector<renderer::Vertex> vertices;
		std::vector<std::vector<UINT>> indices2;

		std::vector<MaterialData> materials;

		bool bAnimation;
		std::vector<std::vector<BoneWeight>> mBoneWeights;
		std::vector<Vector4> skiningWeights;
		std::vector<Vector4> skiningIndices;

		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		std::vector <Microsoft::WRL::ComPtr<ID3D11Buffer>> indicesBuffer;
		D3D11_BUFFER_DESC vbDesc;
		D3D11_BUFFER_DESC ibDesc;
	};

	//struct Bone
	//{
	//	struct KeyFrame
	//	{
	//		Matrix transform;
	//		double time;
	//	};

	//	std::wstring name;
	//	int depth;		//°èÃþ±¸Á¶ ±íÀÌ
	//	int parentIdx; //ºÎ¸ðÀÇ º» ÀÎµ¦½º
	//	Matrix offsetMatrix; // ³ª --> ºÎ¸ð --> »Ñ¸® --> local
	//	Matrix boneMatrix;

	//	std::vector<KeyFrame> keyFrames;
	//};


	//struct AnimationClip
	//{
	//	std::wstring name;
	//	fbxsdk::FbxTime startTime;
	//	fbxsdk::FbxTime endTime;
	//	fbxsdk::FbxLongLong timeLength;
	//	fbxsdk::FbxTime::EMode mode;
	//};
}
