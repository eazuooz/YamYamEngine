#include "yaRenderer.h"
#include "yaApplication.h"
#include "yaResources.h"


#include "yaMesh.h"
#include "yaShader.h"
#include "yaMaterial.h"


namespace ya::renderer
{

	D3D11_INPUT_ELEMENT_DESC InputLayouts[3];
	//std::shared_ptr <Mesh> rectMesh = nullptr;
	//std::shared_ptr<Shader> shader = nullptr;
	//std::shared_ptr <Material> material = nullptr;

	//std::shared_ptr<Mesh> spriteDefaultMesh = nullptr;
	//std::shared_ptr <Material> spriteDefaultMaterial = nullptr;
	//std::shared_ptr<Shader> spriteDefaultShader = nullptr;

	graphics::ConstantBuffer* constantBuffers[(UINT)graphics::eCBType::End];
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)graphics::eSamplerType::End];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizeStates[(UINT)graphics::eRSType::End];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[(UINT)graphics::eDSType::End];
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStateStates[(UINT)graphics::eBSType::End];

	std::vector<Camera*> cameras;


	void LoadShader()
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		Resources::Insert(L"TriangleShader", shader);
		shader->Create(eShaderStage::VS, L"TriangleVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"TrianglePS.hlsl", "main");

		std::shared_ptr<Shader> spriteDefaultShader = std::make_shared<Shader>();
		Resources::Insert(L"SpriteDefaultShader", spriteDefaultShader);
		spriteDefaultShader->Create(eShaderStage::VS, L"SpriteDefaultVS.hlsl", "main");
		spriteDefaultShader->Create(eShaderStage::PS, L"SpriteDefaultPS.hlsl", "main");
		spriteDefaultShader->SetRSState(eRSType::SolidBack);
		spriteDefaultShader->SetDSState(eDSType::Less);
		spriteDefaultShader->SetBSState(eBSType::AlphaBlend);

		std::shared_ptr<Shader> gridShader = std::make_shared<Shader>();
		Resources::Insert(L"GridShader", gridShader);
		gridShader->Create(eShaderStage::VS, L"GridVS.hlsl", "main");
		gridShader->Create(eShaderStage::PS, L"GridPS.hlsl", "main");

		spriteDefaultShader->SetRSState(eRSType::SolidNone);
		spriteDefaultShader->SetDSState(eDSType::NoWrite);
		spriteDefaultShader->SetBSState(eBSType::AlphaBlend);
	}

	void SetUpStates()
	{
		// Input layout 
		InputLayouts[0].AlignedByteOffset = 0;
		InputLayouts[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		InputLayouts[0].InputSlot = 0;
		InputLayouts[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayouts[0].SemanticName = "POSITION";
		InputLayouts[0].SemanticIndex = 0;

		InputLayouts[1].AlignedByteOffset = 16;
		InputLayouts[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		InputLayouts[1].InputSlot = 0;
		InputLayouts[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayouts[1].SemanticName = "COLOR";
		InputLayouts[1].SemanticIndex = 0;

		InputLayouts[2].AlignedByteOffset = 32;
		InputLayouts[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		InputLayouts[2].InputSlot = 0;
		InputLayouts[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayouts[2].SemanticName = "TEXCOORD";
		InputLayouts[2].SemanticIndex = 0;

		std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"TriangleShader");
		GetDevice()->CreateInputLayout(InputLayouts, 3,
			shader->GetVSCode()->GetBufferPointer()
			, shader->GetVSCode()->GetBufferSize()
			, shader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> spriteDefaultShader = Resources::Find<Shader>(L"SpriteDefaultShader");
		GetDevice()->CreateInputLayout(InputLayouts, 3,
			spriteDefaultShader->GetVSCode()->GetBufferPointer()
			, spriteDefaultShader->GetVSCode()->GetBufferSize()
			, spriteDefaultShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> gridShader = Resources::Find<Shader>(L"GridShader");
		GetDevice()->CreateInputLayout(InputLayouts, 3,
			gridShader->GetVSCode()->GetBufferPointer()
			, gridShader->GetVSCode()->GetBufferSize()
			, gridShader->GetInputLayoutAddressOf());


		// Smapler
		D3D11_SAMPLER_DESC desc = {};
		desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSamplerState(&desc, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		GetDevice()->CreateSamplerState(&desc, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());

		GetDevice()->BindsSamplers((UINT)eSamplerType::Anisotropic, 1, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
		GetDevice()->BindsSamplers((UINT)eSamplerType::Point, 1, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());
		
		// rasterrizer
		D3D11_RASTERIZER_DESC rsDesc = {};

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		GetDevice()->CreateRasterizerState(&rsDesc, rasterizeStates[(UINT)eRSType::SolidBack].GetAddressOf());

		
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

		GetDevice()->CreateRasterizerState(&rsDesc, rasterizeStates[(UINT)eRSType::SolidFront].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc, rasterizeStates[(UINT)eRSType::SolidNone].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc, rasterizeStates[(UINT)eRSType::WireframeNone].GetAddressOf());

		// depth stencil
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};

		// less
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;
		
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilStates[(UINT)eDSType::Less].GetAddressOf());

		// Greater
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;
		
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilStates[(UINT)eDSType::Greater].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilStates[(UINT)eDSType::NoWrite].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilStates[(UINT)eDSType::None].GetAddressOf());

		// blend
		blendStateStates[(UINT)eBSType::Default] = nullptr;

		D3D11_BLEND_DESC bsDesc = {};

		// Alpha Blend
		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;
		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		GetDevice()->CreateBlendState(&bsDesc, blendStateStates[(UINT)eBSType::AlphaBlend].GetAddressOf());

		// One One
		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;

		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		GetDevice()->CreateBlendState(&bsDesc, blendStateStates[(UINT)eBSType::OneOne].GetAddressOf());

	}

	void LoadBuffer()
	{
		constantBuffers[(UINT)graphics::eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffers[(UINT)graphics::eCBType::Transform]->Create(sizeof(TransformCB));

		constantBuffers[(UINT)graphics::eCBType::Material] = new ConstantBuffer(eCBType::Material);
		constantBuffers[(UINT)graphics::eCBType::Material]->Create(sizeof(MaterialCB));

		constantBuffers[(UINT)graphics::eCBType::Grid] = new ConstantBuffer(eCBType::Grid);
		constantBuffers[(UINT)graphics::eCBType::Grid]->Create(sizeof(GridCB));
	}

	void LoadMesh()
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

		// Rectangle
		vertexes.resize(4);
		vertexes[0].pos = Vector4(-0.5f, 0.5f, 0.0f, 1.0f);
		vertexes[0].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes[0].uv = Vector2(0.0f, 0.0f);

		vertexes[1].pos = Vector4(0.5f, 0.5f, 0.0f, 1.0f);
		vertexes[1].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		vertexes[1].uv = Vector2(1.0f, 0.0f);

		vertexes[2].pos = Vector4(0.5f, -0.5f, 0.0f, 1.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.f, 1.0f);
		vertexes[2].uv = Vector2(1.0f, 1.0f);

		vertexes[3].pos = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);
		vertexes[3].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[3].uv = Vector2(0.0f, 1.0f);

		std::vector<UINT> indexes;
		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);

		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		// Rect Vertex Buffer
		std::shared_ptr<Mesh> rectMesh = std::make_shared<Mesh>();
		Resources::Insert(L"TriangleMesh", rectMesh);
		rectMesh->CreateVertexBuffer(vertexes.data(), vertexes.size());
		rectMesh->CreateIndexBuffer(indexes.data(), indexes.size());

		std::shared_ptr<Mesh> spriteDefaultMesh = std::make_shared<Mesh>();
		Resources::Insert(L"SpriteDefaultMesh", spriteDefaultMesh);
		spriteDefaultMesh->CreateVertexBuffer(vertexes.data(), vertexes.size());
		spriteDefaultMesh->CreateIndexBuffer(indexes.data(), indexes.size());

		std::shared_ptr<Mesh> gridMesh = std::make_shared<Mesh>();
		Resources::Insert(L"GridMesh", gridMesh);
		gridMesh->CreateVertexBuffer(vertexes.data(), vertexes.size());
		gridMesh->CreateIndexBuffer(indexes.data(), indexes.size());
	}

	void LoadMaterial()
	{
		//Traingle Rect
		std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"TriangleShader");
		std::shared_ptr<Mesh> rectMesh = Resources::Find<Mesh>(L"TriangleMesh");
		std::shared_ptr<Material>	material = std::make_shared<Material>();
		Resources::Insert(L"TriangleMaterial", material);
		material->SetShader(shader);

		std::shared_ptr<Texture> texture
			= Resources::Load<Texture>(L"TriangleTexture", L"..\\Resources\\Triangle.png");
		material->SetTexture(texture);

		int a = 1;
		material->SetData(eGPUParam::Int, &a);

		//Default Sprite
		std::shared_ptr<Shader> spriteDefaultShader = Resources::Find<Shader>(L"SpriteDefaultShader");
		std::shared_ptr<Mesh> spriteDefaultMesh = Resources::Find<Mesh>(L"SpriteDefaultMesh");
		std::shared_ptr<Material>	spriteDefaultMaterial = std::make_shared<Material>();
		spriteDefaultMaterial->SetShader(spriteDefaultShader);
		Resources::Insert(L"SpriteDefaultMaterial", spriteDefaultMaterial);

		std::shared_ptr<Texture> spriteTexture
			= Resources::Load<Texture>(L"SpriteDefaultTexture", L"..\\Resources\\DefaultSprite.png");
		spriteDefaultMaterial->SetTexture(spriteTexture);

		//Grid
		std::shared_ptr<Material> girdMaterial = std::make_shared<Material>();
		Resources::Insert(L"GridMaterial", girdMaterial);
		std::shared_ptr<Shader> gridShader = Resources::Find<Shader>(L"GridShader");
		girdMaterial->SetShader(gridShader);
		
	}

	void Initialize()
	{
		LoadShader();
		SetUpStates();
		LoadBuffer();
		LoadMesh();
		LoadMaterial();
	}

	void Render()
	{
		for (Camera* cam : cameras)
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		cameras.clear();
	}

	void Release()
	{
		for (size_t i = 0; i < (UINT)graphics::eCBType::End; i++)
		{
			if (constantBuffers[i])
			{
				delete constantBuffers[i]; 
				constantBuffers[i] = nullptr;
			}
		}
	}
}