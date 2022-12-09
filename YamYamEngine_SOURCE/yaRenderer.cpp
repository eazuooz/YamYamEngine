#include "yaRenderer.h"
#include "yaGraphicsDevice_DX11.h"
#include "yaApplication.h"

extern ya::Application application;


namespace ya::renderer
{
	Vertex vertexes[3] = {};
	ID3D11Buffer* triangleVertexBuffer = nullptr;
	ID3D11Buffer* triangleConstantBuffer = nullptr;
	ID3D11Buffer* triangleIndexBuffer = nullptr;

	ID3DBlob* errorBlob= nullptr;

	// Blob 은 컴파일된 코드를 저장
	ID3DBlob* triangleVSBlob = nullptr;
	ID3D11VertexShader* triangleVSShader = nullptr;

	ID3DBlob* trianglePSBlob = nullptr;
	ID3D11PixelShader* trianglePSShader = nullptr;

	ID3D11InputLayout* triangleLayout = nullptr;

	void SetUpStates()
	{

	}

	void LoadBuffer()
	{
		// Triangle Vertex Buffer
		D3D11_BUFFER_DESC bufferdesc = {};

		bufferdesc.ByteWidth = sizeof(renderer::Vertex) * 3;
		bufferdesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		bufferdesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		bufferdesc.CPUAccessFlags = 0;

		std::vector<UINT> indexes;
		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);

		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		D3D11_SUBRESOURCE_DATA tSubData = {};
		tSubData.pSysMem = renderer::vertexes;
		GetDevice()->CreateBuffer( &bufferdesc, &tSubData, &triangleVertexBuffer);

		D3D11_BUFFER_DESC idxBufferdesc = {};
		idxBufferdesc.ByteWidth = sizeof(UINT) * indexes.size();
		idxBufferdesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		idxBufferdesc.Usage = D3D11_USAGE_DEFAULT;
		idxBufferdesc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA iSubData = {};
		iSubData.pSysMem = indexes.data();
		GetDevice()->CreateBuffer(&idxBufferdesc, &iSubData, &triangleIndexBuffer);


		D3D11_BUFFER_DESC constantDesc = {};
		constantDesc.ByteWidth = sizeof(Vector4); // 16 의 배수로
		constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		GetDevice()->CreateBuffer(&constantDesc, nullptr, &triangleConstantBuffer);

		Vector4 pos(0.5f, 0.2f, 0.0f, 0.0f);
		GetDevice()->BindConstantBuffer(triangleConstantBuffer, &pos, sizeof(Vector4));
	}

	void LoadShader()
	{
		GetDevice()->CreateShader(ShaderStage::NONE);
	}

	void Initialize()
	{
		vertexes[0].pos = Vector3(0.f, 0.5f, 0.f);
		vertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f);

		vertexes[1].pos = Vector3(0.5f, -0.5f, 0.f);
		vertexes[1].color = Vector4(1.f, 0.f, 0.f, 1.f);

		vertexes[2].pos = Vector3(-0.5f, -0.5f, 0.f);
		vertexes[2].color = Vector4(0.f, 0.f, 1.f, 1.f);

		LoadShader();
		SetUpStates();
		LoadBuffer();
	}

	void Release()
	{
		triangleVertexBuffer->Release();
		errorBlob->Release();
		triangleVSBlob->Release();
		triangleVSShader->Release();
		trianglePSBlob->Release();
		trianglePSShader->Release();
		triangleLayout->Release();
	}
}