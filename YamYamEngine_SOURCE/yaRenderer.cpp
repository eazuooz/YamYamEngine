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
#include "yaTransformBoneShader.h"

namespace ya::renderer
{
	//state
	constexpr int NumElements = 8;
	D3D11_INPUT_ELEMENT_DESC InputLayouts[NumElements] = {};
	graphics::ConstantBuffer* constantBuffers[(UINT)graphics::eCBType::End];
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)graphics::eSamplerType::End];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizeStates[(UINT)graphics::eRSType::End];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[(UINT)graphics::eDSType::End];
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStateStates[(UINT)graphics::eBSType::End];

	// camera
	std::vector<Camera*> cameras[(UINT)eSceneType::End] = {};
	Camera* mainCamera = nullptr;

	// light
	std::vector<LightAttribute> lights = {};
	StructedBuffer* lightsBuffer = nullptr;
	UINT numberOfLight = 0;

	// postProcess
	std::shared_ptr<Texture> postProcessing = nullptr;
	std::shared_ptr<Texture> renderTarget = nullptr;

	// deubg
	std::vector<DebugMesh> debugMeshes = {};

	// gui
	GameObject* inspectorGameObject = nullptr;

	void CreateShader(const std::wstring& name, const std::wstring& fileName
		,eRSType rs = eRSType::SolidBack, eDSType ds = eDSType::Less, eBSType bs = eBSType::Default
		,D3D11_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		const std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		Resources::Insert(name.c_str(), shader);
		shader->Create(eShaderStage::VS, fileName + L"VS.hlsl", "main");
		shader->Create(eShaderStage::HS, fileName + L"HS.hlsl", "main");
		shader->Create(eShaderStage::DS, fileName + L"DS.hlsl", "main");
		shader->Create(eShaderStage::GS, fileName + L"GS.hlsl", "main");
		shader->Create(eShaderStage::PS, fileName + L"PS.hlsl", "main");
		shader->SetRSState(rs);
		shader->SetDSState(ds);
		shader->SetBSState(bs);
		shader->SetTopology(topology);

		GetDevice()->CreateInputLayout(InputLayouts, NumElements,
			shader->GetVSBlob()->GetBufferPointer()
			, shader->GetVSBlob()->GetBufferSize()
			, shader->GetInputLayoutAddressOf());
	}
	void LoadShader()
	{
		//Pipeline shader
		CreateShader(L"SpriteDefaultShader", L"Sprite", eRSType::SolidBack, eDSType::Less, eBSType::AlphaBlend);
		CreateShader(L"DebugShader", L"Debug", eRSType::SolidNone, eDSType::NoWrite, eBSType::AlphaBlend, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		CreateShader(L"GridShader", L"Grid", eRSType::SolidNone, eDSType::NoWrite, eBSType::AlphaBlend);
		CreateShader(L"ParticleShader", L"Particle", eRSType::SolidNone, eDSType::NoWrite, eBSType::AlphaBlend, D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		CreateShader(L"PostProcessShader", L"PostProcess", eRSType::SolidBack, eDSType::NoWrite, eBSType::AlphaBlend);
		CreateShader(L"PhongShader", L"Phong", eRSType::SolidBack, eDSType::Less, eBSType::AlphaBlend);

		/// Compute Shader
		std::shared_ptr<PaintShader> paintShader = std::make_shared<PaintShader>();
		paintShader->Create(L"PaintCS.hlsl", "main");
		Resources::Insert(L"PaintShader", paintShader);

		std::shared_ptr<ParticleShader> particleCS = std::make_shared<ParticleShader>();
		Resources::Insert(L"ParticleShaderCS", particleCS);
		particleCS->Create(L"ParticleCS.hlsl", "main");

		std::shared_ptr<TransformBoneShader> skeleton = std::make_shared<TransformBoneShader>();
		Resources::Insert(L"TransformBoneCS", skeleton);
		skeleton->Create(L"TransformBoneCS.hlsl", "main");
	}
	static int inputLayoutIndex = 0;
	static int inputLayoutOffset = 0;
	void AddInputLayoutVetexData(DXGI_FORMAT format, LPCSTR semanticName)
	{
		InputLayouts[inputLayoutIndex].AlignedByteOffset = inputLayoutOffset;
		InputLayouts[inputLayoutIndex].Format = format;
		InputLayouts[inputLayoutIndex].InputSlot = 0;
		InputLayouts[inputLayoutIndex].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayouts[inputLayoutIndex].SemanticName = semanticName;
		InputLayouts[inputLayoutIndex].SemanticIndex = 0;
		inputLayoutIndex++;

		switch (format)
		{
		case DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT:
			inputLayoutOffset += 4;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT:
			inputLayoutOffset += 8;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT:
			inputLayoutOffset += 12;
			break;
		case DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT:
			inputLayoutOffset += 16;
			break;
		default:
			//
			break;
		}
	}
	void SetUpStates()
	{
		//struct VS_IN
		//{
		//	float3 Position : POSITION;
		//	float3 Color : COLOR;
		//	float2 UV : TEXCOORD;
		//	float3 Normal : NORMAL;
		//	float3 BiNormal : BINORMAL;
		//	float3 Tangent : TANGENT;
		//	float4 BlendWeights : BLENDWEIGHT;
		//	float4 BlendIndices : BLENDINDICES;
		//};
		//};
		AddInputLayoutVetexData(DXGI_FORMAT_R32G32B32_FLOAT, "POSITION");
		AddInputLayoutVetexData(DXGI_FORMAT_R32G32B32A32_FLOAT, "COLOR");
		AddInputLayoutVetexData(DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD");
		AddInputLayoutVetexData(DXGI_FORMAT_R32G32B32_FLOAT, "NORMAL");
		AddInputLayoutVetexData(DXGI_FORMAT_R32G32B32_FLOAT, "BINORMAL");
		AddInputLayoutVetexData(DXGI_FORMAT_R32G32B32_FLOAT, "TANGENT");
		AddInputLayoutVetexData(DXGI_FORMAT_R32G32B32A32_FLOAT, "BLENDWEIGHT");
		AddInputLayoutVetexData(DXGI_FORMAT_R32G32B32A32_FLOAT, "BLENDINDICES");


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
		lightsBuffer->Create(sizeof(LightAttribute), 20, eViewType::SRV, nullptr, true);
	}

	void CreateMesh(const std::wstring& name, std::vector<Vertex>& vertexes, std::vector<UINT>& indices)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		Resources::Insert(name, mesh);
		mesh->CreateMesh(vertexes, indices);
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

		// 윗면
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

		// 아랫면
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

		// 앞면
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

		// 뒷면
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

		// 왼쪽
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

		// 오른쪽
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
			0,  1,  2,  0,  2,  3,  // 윗면
			4,  5,  6,  4,  6,  7,  // 아랫면
			8,  9,  10, 8,  10, 11, // 앞면
			12, 13, 14, 12, 14, 15, // 뒷면
			16, 17, 18, 16, 18, 19, // 왼쪽
			20, 21, 22, 20, 22, 23  // 오른쪽
		};

		CreateMesh(L"CubeMesh", vertices, indices);
	}

	void LoadSphere(const float radius, const int numSlices,
		const int numStacks)
	{
		// 참고: OpenGL Sphere
			// http://www.songho.ca/opengl/gl_sphere.html
			// Texture 좌표계때문에 (numSlices + 1) 개의 버텍스 사용 (마지막에 닫아주는
			// 버텍스가 중복) Stack은 y 위쪽 방향으로 쌓아가는 방식

		const float dTheta = -XM_2PI / float(numSlices);
		const float dPhi = -XM_PI / float(numStacks);


		std::vector<Vertex> vertices = {};

		for (int j = 0; j <= numStacks; j++) {

			// 스택에 쌓일 수록 시작점을 x-y 평면에서 회전 시켜서 위로 올리는 구조
			Vector3 stackStartPoint = Vector3::Transform(
				Vector3(0.0f, -radius, 0.0f), Matrix::CreateRotationZ(dPhi * j));

			for (int i = 0; i <= numSlices; i++) {
				Vertex v;

				// 시작점을 x-z 평면에서 회전시키면서 원을 만드는 구조
				v.pos = Vector3::Transform(
					stackStartPoint, Matrix::CreateRotationY(dTheta * float(i)));

				v.normal = v.pos; // 원점이 구의 중심
				v.normal.Normalize();
				v.uv =
					Vector2(float(i) / numSlices, 1.0f - float(j) / numStacks);

				vertices.push_back(v);
			}
		}

		// cout << vertices.size() << endl;

		std::vector<UINT> indices = {};

		for (int j = 0; j < numStacks; j++) {

			const int offset = (numSlices + 1) * j;

			for (int i = 0; i < numSlices; i++) {

				indices.push_back(offset + i);
				indices.push_back(offset + i + numSlices + 1);
				indices.push_back(offset + i + 1 + numSlices + 1);

				indices.push_back(offset + i);
				indices.push_back(offset + i + 1 + numSlices + 1);
				indices.push_back(offset + i + 1);
			}
		}

		CreateMesh(L"SphereMesh", vertices, indices);
	}

	void MakeCylinder(const float bottomRadius, const float topRadius, float height, int numSlices)
	{

		// Texture 좌표계때문에 (numSlices + 1) x 2 개의 버텍스 사용

		const float dTheta = -XM_2PI / float(numSlices);

		std::vector<Vertex> vertices = {};

		// 옆면의 바닥 버텍스들 (인덱스 0 이상 numSlices 미만)
		for (int i = 0; i <= numSlices; i++) {
			Vertex v;
			v.pos =
				Vector3::Transform(Vector3(bottomRadius, -0.5f * height, 0.0f),
					Matrix::CreateRotationY(dTheta * float(i)));

			// std::cout << v.position.x << " " << v.position.z << std::endl;

			v.normal = v.pos - Vector3(0.0f, -0.5f * height, 0.0f);
			v.normal.Normalize();
			v.uv = Vector2(float(i) / numSlices, 1.0f);

			vertices.push_back(v);
		}

		// 옆면의 맨 위 버텍스들 (인덱스 numSlices 이상 2 * numSlices 미만)
		for (int i = 0; i <= numSlices; i++) {
			Vertex v;
			v.pos =
				Vector3::Transform(Vector3(topRadius, 0.5f * height, 0.0f),
					Matrix::CreateRotationY(dTheta * float(i)));
			v.normal = v.pos - Vector3(0.0f, 0.5f * height, 0.0f);
			v.normal.Normalize();
			v.uv = Vector2(float(i) / numSlices, 0.0f);

			vertices.push_back(v);
		}

		std::vector<UINT> indices = {};

		for (int i = 0; i < numSlices; i++) {
			indices.push_back(i);
			indices.push_back(i + numSlices + 1);
			indices.push_back(i + 1 + numSlices + 1);

			indices.push_back(i);
			indices.push_back(i + 1 + numSlices + 1);
			indices.push_back(i + 1);
		}

		CreateMesh(L"CylinderMesh", vertices, indices);
	}

	void LoadMesh()
	{
		LoadPoint();
		LoadRect();
		LoadCircle();
		LoadCube();
		LoadSphere(1.5f, 15, 13);
		MakeCylinder(1.5, 1.5, 1.5, 15);

		std::shared_ptr<Mesh> zelda 
			= Resources::Load<Mesh>(L"Zelda", L"..\\Resources\\zelda\\zeldaPosed001.fbx");
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
	void CreateMaterial(const std::wstring& name, const std::wstring& ShaderName, eRenderingMode mode = eRenderingMode::Opaque)
	{
		std::shared_ptr<Shader> shader = Resources::Find<Shader>(ShaderName);
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(shader);
		material->SetRenderingMode(mode);
		Resources::Insert(name, material);
	}
	void LoadMaterial()
	{
		CreateMaterial(L"SpriteMaterial", L"SpriteDefaultShader");
		CreateMaterial(L"GridMaterial", L"GridShader");
		CreateMaterial(L"DebugMaterial", L"DebugShader");
		CreateMaterial(L"ParticleMaterial", L"ParticleShader", eRenderingMode::Transparent);
		CreateMaterial(L"PostProcessMaterial", L"PostProcessShader", eRenderingMode::PostProcess);
		CreateMaterial(L"PhongMaterial", L"PhongShader");
	}

	void Initialize()
	{
		SetUpStates();
		LoadShader();
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
	static float noiseTime = 5.0f;
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
		lightsBuffer->SetData(lights.data(), lights.size());
		lightsBuffer->BindSRV(eShaderStage::VS, static_cast<UINT>(eStructedBuffer::Lights));
		lightsBuffer->BindSRV(eShaderStage::PS, static_cast<UINT>(eStructedBuffer::Lights));
		numberOfLight = lights.size();

		renderer::LightCB data = {};
		data.lightCount = lights.size();

		ConstantBuffer* cb = renderer::constantBuffers[static_cast<UINT>(graphics::eCBType::Light)];
		cb->SetData(&data);
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