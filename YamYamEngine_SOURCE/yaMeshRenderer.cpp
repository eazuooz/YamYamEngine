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
		GetOwner()->GetComponent<Transform>()->Bind();

		std::shared_ptr<Mesh> mesh = GetMesh();

		if (mesh->GetMeshDatas()[0]->materials.size() == 0)
		{
			MeshData::MaterialData matData = {};
			matData.name = GetMaterial()->GetKey();

			mesh->GetMeshDatas()[0]->materials.push_back(matData);
		}


		mesh->Render();
	}
}

