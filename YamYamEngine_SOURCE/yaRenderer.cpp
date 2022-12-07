#include "yaRenderer.h"
#include "yaGraphicsDevice_DX11.h"
#include "yaApplication.h"

extern ya::Application application;


namespace ya::renderer
{
	Vertex vertexes[3] = {};
	ID3D11Buffer* triangleBuffer = nullptr;
	
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
		GetDevice()->CreateBuffer(&triangleBuffer, nullptr, nullptr);
	}

	void LoadShader()
	{
		GetDevice()->CreateShader();
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
		triangleBuffer->Release();
		errorBlob->Release();
		triangleVSBlob->Release();
		triangleVSShader->Release();
		trianglePSBlob->Release();
		trianglePSShader->Release();
		triangleLayout->Release();
	}
}