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
		Transform* transform = GetOwner()->GetComponent<Transform>();
		Animator* animator = GetOwner()->GetComponent<Animator>();
		std::shared_ptr<Mesh> mesh = GetMesh();

		std::vector<MeshData*> meshDatas = mesh->GetMeshDatas();

		if (transform)
			transform->Bind();

		if (animator)
			animator->Bind(meshDatas[0]->bones.size());

		mesh->Render();

		if (animator)
			animator->Clear();
	}
}
