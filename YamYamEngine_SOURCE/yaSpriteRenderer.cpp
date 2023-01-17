#include "yaSpriteRenderer.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaResources.h"


namespace ya
{
	SpriteRenderer::SpriteRenderer()
		: Component(eComponentType::Mesh)
	{
		InitializeResource();
	}

	SpriteRenderer::~SpriteRenderer()
	{

	}

	void SpriteRenderer::Initialize()
	{
		
	}

	void SpriteRenderer::Update()
	{
	}

	void SpriteRenderer::FixedUpdate()
	{
	}

	void SpriteRenderer::Render()
	{
		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

		mMaterial->Bind();

		mMesh->Render();
	}
	void SpriteRenderer::InitializeResource()
	{
		mMesh = Resources::Find<Mesh>(L"SpriteDefaultMesh");
		mMaterial = Resources::Find<Material>(L"SpriteDefaultMaterial");
	}
}