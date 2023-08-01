#include "yaRenderer.h"
#include "yaApplication.h"
#include "yaResources.h"

#include "yaTime.h"
#include "yaMesh.h"
#include "yaShader.h"
#include "yaMaterial.h"
#include "yaSceneManager.h"
#include "yaComputeShader.h"
#include "yaPaintShader.h"
#include "yaParticleShader.h"

namespace ya::renderer
{
	D3D11_INPUT_ELEMENT_DESC InputLayouts[6] = {};

	graphics::ConstantBuffer* constantBuffers[(UINT)graphics::eCBType::End];
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)graphics::eSamplerType::End];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizeStates[(UINT)graphics::eRSType::End];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[(UINT)graphics::eDSType::End];
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStateStates[(UINT)graphics::eBSType::End];

	std::vector<Camera*> cameras[(UINT)eSceneType::End];
	Camera* mainCamera = nullptr;
	std::vector<LightAttribute> lights;
	StructedBuffer* lightsBuffer = nullptr;
	UINT numberOfLight = 0;
	std::shared_ptr<Texture> postProcessing = nullptr;
	std::shared_ptr<Texture> renderTarget = nullptr;

	std::vector<DebugMesh> debugMeshes;
	GameObject* inspectorGameObject = nullptr;

	void LoadShader()
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		Resources::Insert(L"TriangleShader", shader);
		shader->Create(eShaderStage::VS, L"TriangleVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"TrianglePS.hlsl", "main");

		std::shared_ptr<Shader> spriteDefaultShader = std::make_shared<Shader>();
		Resources::Insert(L"SpriteDefaultShader", spriteDefaultShader);
		spriteDefaultShader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		spriteDefaultShader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");
		spriteDefaultShader->SetRSState(eRSType::SolidBack);
		spriteDefaultShader->SetDSState(eDSType::Less);
		spriteDefaultShader->SetBSState(eBSType::AlphaBlend);

		std::shared_ptr<Shader> gridShader = std::make_shared<Shader>();
		Resources::Insert(L"GridShader", gridShader);
		gridShader->Create(eShaderStage::VS, L"GridVS.hlsl", "main");
		gridShader->Create(eShaderStage::PS, L"GridPS.hlsl", "main");

		gridShader->SetRSState(eRSType::SolidNone);
		gridShader->SetDSState(eDSType::NoWrite);
		gridShader->SetBSState(eBSType::AlphaBlend);

		std::shared_ptr<Shader> debugShader = std::make_shared<Shader>();
		Resources::Insert(L"DebugShader", debugShader);
		debugShader->Create(eShaderStage::VS, L"DebugVS.hlsl", "main");
		debugShader->Create(eShaderStage::PS, L"DebugPS.hlsl", "main");
		debugShader->SetRSState(eRSType::SolidNone);
		debugShader->SetDSState(eDSType::NoWrite);
		debugShader->SetBSState(eBSType::AlphaBlend);
		debugShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		std::shared_ptr<PaintShader> paintShader = std::make_shared<PaintShader>();
		paintShader->Create(L"PaintCS.hlsl", "main");
		Resources::Insert(L"PaintShader", paintShader);

		std::shared_ptr<Shader> particleShader = std::make_shared<Shader>();
		Resources::Insert(L"ParticleShader", particleShader);
		particleShader->Create(eShaderStage::VS, L"ParticleVS.hlsl", "main");
		particleShader->Create(eShaderStage::PS, L"ParticlePS.hlsl", "main");
		particleShader->Create(eShaderStage::GS, L"ParticleGS.hlsl", "main");
		particleShader->SetRSState(eRSType::SolidNone);
		particleShader->SetDSState(eDSType::NoWrite);
		particleShader->SetBSState(eBSType::AlphaBlend);
		particleShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


		//CS
		std::shared_ptr<ParticleShader> particleCS = std::make_shared<ParticleShader>();
		Resources::Insert(L"ParticleShaderCS", particleCS);
		particleCS->Create(L"ParticleCS.hlsl", "main");

		//Post Process
		std::shared_ptr<Shader> postProcessShader = std::make_shared<Shader>();
		Resources::Insert(L"PostProcessShader", postProcessShader);
		postProcessShader->Create(eShaderStage::VS, L"PostProcessVS.hlsl", "main");
		postProcessShader->Create(eShaderStage::PS, L"PostProcessPS.hlsl", "main");
		postProcessShader->SetDSState(eDSType::NoWrite);
	}

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

		InputLayouts[3].AlignedByteOffset = 36;
		InputLayouts[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		InputLayouts[3].InputSlot = 0;
		InputLayouts[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayouts[3].SemanticName = "TANGENT";
		InputLayouts[3].SemanticIndex = 0;

		InputLayouts[3].AlignedByteOffset = 48;
		InputLayouts[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		InputLayouts[3].InputSlot = 0;
		InputLayouts[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayouts[3].SemanticName = "BINORMAL";
		InputLayouts[3].SemanticIndex = 0;

		InputLayouts[3].AlignedByteOffset = 50;
		InputLayouts[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		InputLayouts[3].InputSlot = 0;
		InputLayouts[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayouts[3].SemanticName = "NORMAL";
		InputLayouts[3].SemanticIndex = 0;

		std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"TriangleShader");
		GetDevice()->CreateInputLayout(InputLayouts, 3,
			shader->GetVSBlob()->GetBufferPointer()
			, shader->GetVSBlob()->GetBufferSize()
			, shader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> spriteDefaultShader = Resources::Find<Shader>(L"SpriteDefaultShader");
		GetDevice()->CreateInputLayout(InputLayouts, 3,
			spriteDefaultShader->GetVSBlob()->GetBufferPointer()
			, spriteDefaultShader->GetVSBlob()->GetBufferSize()
			, spriteDefaultShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> gridShader = Resources::Find<Shader>(L"GridShader");
		GetDevice()->CreateInputLayout(InputLayouts, 3,
			gridShader->GetVSBlob()->GetBufferPointer()
			, gridShader->GetVSBlob()->GetBufferSize()
			, gridShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> debugShader = Resources::Find<Shader>(L"DebugShader");
		GetDevice()->CreateInputLayout(InputLayouts, 3,
			debugShader->GetVSBlob()->GetBufferPointer()
			, debugShader->GetVSBlob()->GetBufferSize()
			, debugShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> particleShader = Resources::Find<Shader>(L"ParticleShader");
		GetDevice()->CreateInputLayout(InputLayouts, 3,
			particleShader->GetVSBlob()->GetBufferPointer()
			, particleShader->GetVSBlob()->GetBufferSize()
			, particleShader->GetInputLayoutAddressOf());


		std::shared_ptr<Shader> postProcessShader = Resources::Find<Shader>(L"PostProcessShader");
		GetDevice()->CreateInputLayout(InputLayouts, 3,
			postProcessShader->GetVSBlob()->GetBufferPointer()
			, postProcessShader->GetVSBlob()->GetBufferSize()
			, postProcessShader->GetInputLayoutAddressOf());

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

		rsDesc.MultisampleEnable = true;
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		GetDevice()->CreateRasterizerState(&rsDesc, rasterizeStates[(UINT)eRSType::SolidBack].GetAddressOf());

		rsDesc.MultisampleEnable = true;
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

		GetDevice()->CreateRasterizerState(&rsDesc, rasterizeStates[(UINT)eRSType::SolidFront].GetAddressOf());

		rsDesc.MultisampleEnable = true;
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc, rasterizeStates[(UINT)eRSType::SolidNone].GetAddressOf());
		rsDesc.MultisampleEnable = true;
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc, rasterizeStates[(UINT)eRSType::WireframeNone].GetAddressOf());

		// depth stencil
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};

		// less
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
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
		//constant buffer
		constantBuffers[(UINT)graphics::eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffers[(UINT)graphics::eCBType::Transform]->Create(sizeof(TransformCB));

		constantBuffers[(UINT)graphics::eCBType::Material] = new ConstantBuffer(eCBType::Material);
		constantBuffers[(UINT)graphics::eCBType::Material]->Create(sizeof(MaterialCB));

		constantBuffers[(UINT)graphics::eCBType::Camera] = new ConstantBuffer(eCBType::Camera);
		constantBuffers[(UINT)graphics::eCBType::Camera]->Create(sizeof(CameraCB));

		constantBuffers[(UINT)graphics::eCBType::Animator] = new ConstantBuffer(eCBType::Animator);
		constantBuffers[(UINT)graphics::eCBType::Animator]->Create(sizeof(AnimatorCB));

		constantBuffers[(UINT)graphics::eCBType::Light] = new ConstantBuffer(eCBType::Light);
		constantBuffers[(UINT)graphics::eCBType::Light]->Create(sizeof(LightCB));

		constantBuffers[(UINT)graphics::eCBType::ParticleSystem] = new ConstantBuffer(eCBType::ParticleSystem);
		constantBuffers[(UINT)graphics::eCBType::ParticleSystem]->Create(sizeof(ParticleSystemCB));

		constantBuffers[(UINT)graphics::eCBType::Noise] = new ConstantBuffer(eCBType::Noise);
		constantBuffers[(UINT)graphics::eCBType::Noise]->Create(sizeof(NoiseCB));

		//Structed buffer
		lightsBuffer = new StructedBuffer();
		lightsBuffer->Create(sizeof(LightAttribute), 1, eViewType::SRV, nullptr, true);
	}

	void LoadMesh()
	{
		LoadPoint();
		LoadRect();
		LoadCircle();
		LoadCube();
	}

	void LoadTexture()
	{
		//Post processing
		postProcessing = std::make_shared<Texture>();
		postProcessing->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		postProcessing->BindShaderResource(eShaderStage::PS, 60);

		// Particle
		Resources::Load<Texture>(L"SpriteDefaultTexture", L"..\\Resources\\DefaultSprite.png");
		Resources::Load<Texture>(L"TriangleTexture", L"..\\Resources\\Triangle.png");
		Resources::Load<Texture>(L"SmokeParticle", L"..\\Resources\\particle\\CartoonSmoke.png");
		Resources::Load<Texture>(L"BubbleParticle", L"..\\Resources\\particle\\Bubbles50px.png");

		// Noise
		Resources::Load<Texture>(L"Noise01", L"..\\Resources\\noise\\noise_01.png");
		Resources::Load<Texture>(L"Noise02", L"..\\Resources\\noise\\noise_02.png");
		Resources::Load<Texture>(L"Noise03", L"..\\Resources\\noise\\noise_03.jpg");
	}

	void LoadMaterial()
	{
		//Rect
		std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"TriangleShader");
		std::shared_ptr<Mesh> rectMesh = Resources::Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material>	material = std::make_shared<Material>();
		Resources::Insert(L"TriangleMaterial", material);
		material->SetShader(shader);

		std::shared_ptr<Texture> texture
			= Resources::Find<Texture>(L"TriangleTexture");
		material->SetTexture(eTextureType::Albedo, texture);

		int a = 1;
		material->SetData(eGPUParam::Int, &a);

		//Default Sprite
		std::shared_ptr<Shader> spriteDefaultShader = Resources::Find<Shader>(L"SpriteDefaultShader");
		std::shared_ptr<Mesh> spriteDefaultMesh = Resources::Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material>	spriteDefaultMaterial = std::make_shared<Material>();
		spriteDefaultMaterial->SetShader(spriteDefaultShader);
		Resources::Insert(L"SpriteDefaultMaterial", spriteDefaultMaterial);

		//Camera
		std::shared_ptr<Material> girdMaterial = std::make_shared<Material>();
		Resources::Insert(L"GridMaterial", girdMaterial);
		std::shared_ptr<Shader> gridShader = Resources::Find<Shader>(L"GridShader");
		girdMaterial->SetShader(gridShader);

		// debug
		std::shared_ptr<Material> debugMaterial = std::make_shared<Material>();
		debugMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"DebugMaterial", debugMaterial);
		std::shared_ptr<Shader> debugShader = Resources::Find<Shader>(L"DebugShader");
		debugMaterial->SetShader(debugShader);

		// particle
		std::shared_ptr<Material> particleMaterial = std::make_shared<Material>();
		particleMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert(L"ParticleMaterial", particleMaterial);
		std::shared_ptr<Shader> particleShader = Resources::Find<Shader>(L"ParticleShader");
		particleMaterial->SetShader(particleShader);

		// postProcess
		std::shared_ptr<Material> postProcessMaterial = std::make_shared<Material>();
		postProcessMaterial->SetRenderingMode(eRenderingMode::PostProcess);
		Resources::Insert(L"PostProcessMaterial", postProcessMaterial);
		std::shared_ptr<Shader> postProcessShader = Resources::Find<Shader>(L"PostProcessShader");
		postProcessMaterial->SetShader(postProcessShader);
	}

	void CreateMesh(const std::wstring& name, std::vector<Vertex>& vertexes, std::vector<UINT>& indices)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		Resources::Insert(name, mesh);
		mesh->CreateVertexBuffer(vertexes.data(), vertexes.size());
		mesh->CreateIndexBuffer(indices.data() , indices.size());
	}

	void LoadPoint()
	{
		std::vector<Vertex> vertexes;
		std::vector<UINT> indices;

		Vertex v = {};
		v.pos = Vector3(0.0f, 0.0f, 0.0f);
		v.color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		v.uv = Vector2(0.0f, 0.0f);
		vertexes.push_back(v);

		UINT idx = 0;
		indices.push_back(idx);

		CreateMesh(L"PointMesh", vertexes, indices);
	}

	void LoadRect()
	{
		std::vector<Vertex> vertexes;
		std::vector<UINT> indices;

		vertexes.resize(4);
		vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes[0].uv = Vector2(0.0f, 0.0f);

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
		vertexes[1].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		vertexes[1].uv = Vector2(1.0f, 0.0f);

		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.f, 1.0f);
		vertexes[2].uv = Vector2(1.0f, 1.0f);

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[3].uv = Vector2(0.0f, 1.0f);

		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		CreateMesh(L"RectMesh", vertexes, indices);
	}

	void LoadCircle()
	{
		std::vector<Vertex> vertices;
		std::vector<UINT> indices;

		Vertex center = {};
		center.pos = Vector3(0.0f, 0.0f, 0.0f);
		center.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		center.uv = Vector2(0.5f, 0.5f);
		vertices.push_back(center);

		int degree = 360;
		float radian = XM_2PI / (float)degree;
		float radius = 0.5f;

		for (int i = 0; i < degree; ++i)
		{
			center.pos = Vector3(radius * cosf(radian * (float)i), radius * sinf(radian * (float)i), 0.0f);
			center.uv = Vector2(center.pos.x + 0.5f, 1.0f - (center.pos.y + 0.5f));
			center.color = Vector4(0.0f, 1.0f, 0.0f, 1.f);
			vertices.push_back(center);
		}

		for (int i = 0; i < vertices.size() - 2; ++i)
			indices.push_back(i + 1);
		
		indices.push_back(1);

		CreateMesh(L"CircleMesh", vertices, indices);
	}

	void LoadCube()
	{
		std::vector<Vector3> positions;
		std::vector<Vector3> colors;
		std::vector<Vector3> normals;
		std::vector<Vector2> texcoords; 

		const float scale = 1.0f;

		// À­¸é
		positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
		positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
		positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
		colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
		colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
		colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
		colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
		normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
		normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
		normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
		normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
		texcoords.push_back(Vector2(0.0f, 0.0f));
		texcoords.push_back(Vector2(1.0f, 0.0f));
		texcoords.push_back(Vector2(1.0f, 1.0f));
		texcoords.push_back(Vector2(0.0f, 1.0f));

		// ¾Æ·§¸é
		positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
		positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
		positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
		colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
		colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
		colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
		colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
		normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
		normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
		normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
		normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
		texcoords.push_back(Vector2(0.0f, 0.0f));
		texcoords.push_back(Vector2(1.0f, 0.0f));
		texcoords.push_back(Vector2(1.0f, 1.0f));
		texcoords.push_back(Vector2(0.0f, 1.0f));

		// ¾Õ¸é
		positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
		positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
		colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
		colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
		colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
		colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
		normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
		normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
		normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
		normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
		texcoords.push_back(Vector2(0.0f, 0.0f));
		texcoords.push_back(Vector2(1.0f, 0.0f));
		texcoords.push_back(Vector2(1.0f, 1.0f));
		texcoords.push_back(Vector2(0.0f, 1.0f));

		// µÞ¸é
		positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
		positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
		colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
		colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
		colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
		colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
		normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
		normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
		normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
		normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
		texcoords.push_back(Vector2(0.0f, 0.0f));
		texcoords.push_back(Vector2(1.0f, 0.0f));
		texcoords.push_back(Vector2(1.0f, 1.0f));
		texcoords.push_back(Vector2(0.0f, 1.0f));

		// ¿ÞÂÊ
		positions.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
		positions.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
		colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
		colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
		colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
		colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
		normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
		normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
		normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
		normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
		texcoords.push_back(Vector2(0.0f, 0.0f));
		texcoords.push_back(Vector2(1.0f, 0.0f));
		texcoords.push_back(Vector2(1.0f, 1.0f));
		texcoords.push_back(Vector2(0.0f, 1.0f));

		// ¿À¸¥ÂÊ
		positions.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
		positions.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
		positions.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
		positions.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
		colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
		colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
		colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
		colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
		normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
		normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
		normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
		normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
		texcoords.push_back(Vector2(0.0f, 0.0f));
		texcoords.push_back(Vector2(1.0f, 0.0f));
		texcoords.push_back(Vector2(1.0f, 1.0f));
		texcoords.push_back(Vector2(0.0f, 1.0f));

		std::vector<Vertex> vertices;
		for (size_t i = 0; i < positions.size(); i++) 
		{
			Vertex v;
			v.pos = positions[i];
			v.normal = normals[i];
			v.uv = texcoords[i];
			// v.color = colors[i];
			vertices.push_back(v);
		}

		std::vector<UINT> indices = 
		{
			0,  1,  2,  0,  2,  3,  // À­¸é
			4,  5,  6,  4,  6,  7,  // ¾Æ·§¸é
			8,  9,  10, 8,  10, 11, // ¾Õ¸é
			12, 13, 14, 12, 14, 15, // µÞ¸é
			16, 17, 18, 16, 18, 19, // ¿ÞÂÊ
			20, 21, 22, 20, 22, 23  // ¿À¸¥ÂÊ
		};

		CreateMesh(L"CubeMesh", vertices, indices);
	}

	void Initialize()
	{
		LoadShader();
		SetUpStates();
		LoadBuffer();
		LoadTexture();
		LoadMesh();
		LoadMaterial();
	}

	void Render()
	{
		BindNoiseTexture();
		BindLights();
		
		eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		for (Camera* cam : cameras[(UINT)type])
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		cameras[(UINT)type].clear();
		renderer::lights.clear();
	}
	
	void DebugRender()
	{

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

		delete lightsBuffer;
		lightsBuffer = nullptr;
	}

	void PushDebugMesh(DebugMesh& mesh)
	{
		debugMeshes.push_back(mesh);
	}

	void PushLightAttribute(LightAttribute& lightAttribute)
	{
		lights.push_back(lightAttribute);
	}

	float noiseTime = 5.0f;
	void BindNoiseTexture()
	{
		std::shared_ptr<Texture> noise = Resources::Find<Texture>(L"Noise03");
		noise->BindShaderResource(eShaderStage::VS, 13);
		noise->BindShaderResource(eShaderStage::HS, 13);
		noise->BindShaderResource(eShaderStage::DS, 13);
		noise->BindShaderResource(eShaderStage::GS, 13);
		noise->BindShaderResource(eShaderStage::PS, 13);
		noise->BindShaderResource(eShaderStage::CS, 13);

		noiseTime -= Time::DeltaTime();

		NoiseCB info = {};
		info.size.x = noise->GetWidth();
		info.size.y = noise->GetHeight();
		info.noiseTime = noiseTime;

		ConstantBuffer* cb = constantBuffers[(UINT)eCBType::Noise];
		cb->SetData(&info);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);


	}

	void BindLights()
	{
		if (lightsBuffer->GetStride() < (UINT)lights.size())
		{
			lightsBuffer->Create(lightsBuffer->GetSize(), (UINT)lights.size(), eViewType::SRV, nullptr);
		}

		lightsBuffer->SetData(lights.data(), lights.size());
		lightsBuffer->BindSRV(eShaderStage::VS, 14);
		lightsBuffer->BindSRV(eShaderStage::PS, 14);
		numberOfLight = lights.size();

		renderer::LightCB trCB = {};
		trCB.numberOfLight = lights.size();

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)graphics::eCBType::Light];
		cb->SetData(&trCB);
		cb->Bind(graphics::eShaderStage::VS);
		cb->Bind(graphics::eShaderStage::PS);
	}

	void CopyRenderTarget()
	{
		if (renderTarget == nullptr)
			return;

		ID3D11ShaderResourceView* srv = nullptr;
		GetDevice()->BindShaderResource(eShaderStage::PS, 60, &srv);

		GetDevice()->CopyResource(postProcessing->GetTexture().Get()
			, renderTarget->GetTexture().Get());

		postProcessing->BindShaderResource(eShaderStage::PS, 60);
	}
}