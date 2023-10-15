#include "yaSkinnedMeshRenderer.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaAnimator.h"

namespace ya
{
	SkinnedMeshRenderer::SkinnedMeshRenderer()
		: BaseRenderer(eComponentType::SkinnedMeshRenderer)
	{
	}

	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{
	}

	void SkinnedMeshRenderer::Initialize()
	{

	}

	void SkinnedMeshRenderer::Update()
	{

	}

	void SkinnedMeshRenderer::LateUpdate()
	{

	}

	void SkinnedMeshRenderer::Render()
	{
		GetOwner()->GetComponent<Transform>()->BindConstantBuffer();
		GetMesh()->Render();
	}
}
