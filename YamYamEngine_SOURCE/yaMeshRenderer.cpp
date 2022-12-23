#include "yaMeshRenderer.h"
#include "yaGameObject.h"
#include "yaTransform.h"

namespace ya
{
	MeshRenderer::MeshRenderer()
		: Component(eComponentType::Mesh)
	{
	}

	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::Initialize()
	{
	}

	void MeshRenderer::Update()
	{
	}

	void MeshRenderer::FixedUpdate()
	{
	}

	void MeshRenderer::Render()
	{
		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

		mShader->Update();
		mMesh->Render();
	}
}

