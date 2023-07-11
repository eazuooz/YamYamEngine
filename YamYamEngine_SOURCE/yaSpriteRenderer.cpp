#include "yaSpriteRenderer.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaResources.h"
#include "yaAnimator.h"

namespace ya
{
	SpriteRenderer::SpriteRenderer()
		: BaseRenderer(eComponentType::SpriteRenderer)
	{
		initializeResource();
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

	void SpriteRenderer::LateUpdate()
	{
	}

	void SpriteRenderer::Render()
	{
		GetOwner()->GetComponent<Transform>()->BindConstantBuffer();
		GetMaterial()->Bind();

		Animator* animator = GetOwner()->GetComponent<Animator>();
		if (animator)
			animator->Binds();

		GetMesh()->Render();
		GetMaterial()->Clear();

		if (animator)
			animator->Clear();
	}

	void SpriteRenderer::initializeResource()
	{
		SetMesh(Resources::Find<Mesh>(L"SpriteDefaultMesh"));
		SetMaterial(Resources::Find<Material>(L"SpriteDefaultMaterial"));
	}
}