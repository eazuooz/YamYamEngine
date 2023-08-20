#include "BaseRenderer.h"
#include "yaResources.h"


namespace ya
{
	BaseRenderer::BaseRenderer(eComponentType type)
		: Component(type)
	{
	}

	BaseRenderer::~BaseRenderer()
	{
	}

	void BaseRenderer::Initialize()
	{
	}

	void BaseRenderer::Update()
	{
	}

	void BaseRenderer::LateUpdate()
	{
	}

	void BaseRenderer::Render()
	{
	}

	void BaseRenderer::SetMesh(const std::wstring& name)
	{
		std::shared_ptr<Mesh> mesh
			= Resources::Find<Mesh>(name);

		mMesh = mesh;
	}

	void BaseRenderer::SetMaterial(const std::wstring& name)
	{
		std::shared_ptr<Material> material
			= Resources::Find<Material>(name);

		mMaterial = material;
	}
}
