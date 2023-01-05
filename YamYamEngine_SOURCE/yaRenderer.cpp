#include "yaRenderer.h"
#include "yaApplication.h"
#include "yaResources.h"
#include "yaMaterial.h"

namespace ya::renderer
{

	D3D11_INPUT_ELEMENT_DESC InputLayouts[3];
	std::shared_ptr < Mesh> mesh = nullptr;
	std::shared_ptr<Shader> shader = nullptr;
	std::shared_ptr < Material> material = nullptr;

	ConstantBuffer* constantBuffers[(UINT)graphics::eCBType::End];
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)graphics::eSamplerType::End];

	void SetUpStates()
	{
		// Input layout 
		InputLayouts[0].AlignedByteOffset = 0;
		InputLayouts[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		InputLayouts[0].InputSlot = 0;
		InputLayouts[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayouts[0].SemanticName = "POSITION";
		InputLayouts[0].SemanticIndex = 0;

		InputLayouts[1].AlignedByteOffset = 12;
		InputLayouts[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		InputLayouts[1].InputSlot = 0;
		InputLayouts[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayouts[1].SemanticName = "COLOR";
		InputLayouts[1].SemanticIndex = 0;

		InputLayouts[2].AlignedByteOffset = 28;
		InputLayouts[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		InputLayouts[2].InputSlot = 0;
		InputLayouts[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayouts[2].SemanticName = "TEXCOORD";
		InputLayouts[2].SemanticIndex = 0;

		GetDevice()->CreateInputLayout(InputLayouts, 3,
			shader->GetVSCode()->GetBufferPointer()
			, shader->GetVSCode()->GetBufferSize()
			, shader->GetInputLayoutAddressOf());

		// Smapler
		D3D11_SAMPLER_DESC desc = {};
		desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSampler(&desc, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		GetDevice()->CreateSampler(&desc, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());

		GetDevice()->BindsSamplers((UINT)eSamplerType::Anisotropic, 1, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
		GetDevice()->BindsSamplers((UINT)eSamplerType::Point, 1, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());
	}

	void LoadBuffer()
	{
		// Triangle
		std::vector<Vertex> vertexes;
		
		//vertexes.resize(3);
		//vertexes[0].pos = Vector3(0.f, 0.5f, 0.f);
		//vertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f);
		//vertexes[0].uv = Vector2(0.5f, 0.0f);

		//vertexes[1].pos = Vector3(0.5f, -0.5f, 0.f);
		//vertexes[1].color = Vector4(1.f, 0.f, 0.f, 1.f);
		//vertexes[1].uv = Vector2(1.0f, 1.0f);

		//vertexes[2].pos = Vector3(-0.5f, -0.5f, 0.f);
		//vertexes[2].color = Vector4(0.f, 0.f, 1.f, 1.f);
		//vertexes[2].uv = Vector2(0.0f, 1.0f);

		vertexes.resize(4);
		vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.f);
		vertexes[0].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes[0].uv = Vector2(0.0f, 0.0f);

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.f);
		vertexes[1].color = Vector4(1.f, 0.f, 0.f, 1.f);
		vertexes[1].uv = Vector2(1.0f, 0.0f);

		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.f);
		vertexes[2].color = Vector4(0.f, 0.f, 1.f, 1.f);
		vertexes[2].uv = Vector2(1.0f, 1.0f);

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.f);
		vertexes[3].color = Vector4(0.f, 0.f, 1.f, 1.f);
		vertexes[3].uv = Vector2(0.0f, 1.0f);

		std::vector<UINT> indexes;
		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);

		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		//// Triangle Vertex Buffer
		//mesh->CreateVertexBuffer(vertexes.data(), 3);
		//mesh->CreateIndexBuffer(indexes.data(), indexes.size());

		// Rect Vertex Buffer
		mesh->CreateVertexBuffer(vertexes.data(), vertexes.size());
		mesh->CreateIndexBuffer(indexes.data(), indexes.size());

		Resources::Insert(L"TriangleMesh", mesh);

		constantBuffers[(UINT)graphics::eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffers[(UINT)graphics::eCBType::Transform]->Create(sizeof(TransformCB));

		constantBuffers[(UINT)graphics::eCBType::Material] = new ConstantBuffer(eCBType::Material);
		constantBuffers[(UINT)graphics::eCBType::Material]->Create(sizeof(MaterialCB));
		
	}

	void LoadShader()
	{
		shader->Create(eShaderStage::VS, L"TriangleVS.hlsl", "VS_Test");
		shader->Create(eShaderStage::PS, L"TrianglePS.hlsl", "PS_Test");
		Resources::Insert(L"TriangleShader", shader);


	}

	void Initialize()
	{
		mesh = std::make_shared<Mesh>(); 
		shader = std::make_shared<Shader>(); 
		material = std::make_shared<Material>(); 

		LoadShader();
		SetUpStates();
		LoadBuffer();

		material->SetShader(shader);
		Resources::Insert(L"TriangleMaterial", material);

		int a = 1;
		material->SetData(eGPUParam::Int, &a);
	}

	void Release()
	{
		//delete mesh;
		//delete shader;

		for (size_t i = 0; i < (UINT)graphics::eCBType::End; i++)
		{
			if (constantBuffers[i])
			{
				delete constantBuffers[i]; 
				constantBuffers[i] = nullptr;
			}
		}

		//Resources::Release();
	}
}