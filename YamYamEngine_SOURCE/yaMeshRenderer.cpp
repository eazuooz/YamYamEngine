#include "yaMeshRenderer.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaAnimator.h"

namespace ya
{
	MeshRenderer::MeshRenderer()
		: BaseRenderer(eComponentType::MeshRenderer)
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

	void MeshRenderer::LateUpdate()
	{
	}

	void MeshRenderer::Render()
	{
		GetOwner()->GetComponent<Transform>()->BindConstantBuffer();
		Animator* animator = GetOwner()->GetComponent<Animator>();
		if (animator)
			animator->Binds();

		GetMesh()->Render();
		//materials[i]->Clear();

		if (animator)
			animator->Clear();
	}
}

