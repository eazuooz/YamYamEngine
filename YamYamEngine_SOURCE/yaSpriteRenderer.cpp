#include "yaSpriteRenderer.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaResources.h"


namespace ya
{
	SpriteRenderer::SpriteRenderer()
		: BaseRenderer(eComponentType::SpriteRenderer)
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

		GetMaterial()->Bind();

		GetMesh()->Render();

		GetMaterial()->Clear();
	}
	void SpriteRenderer::InitializeResource()
	{
		SetMesh(Resources::Find<Mesh>(L"SpriteDefaultMesh"));
		SetMaterial(Resources::Find<Material>(L"SpriteDefaultMaterial"));
	}
}