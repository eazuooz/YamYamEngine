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
		std::shared_ptr<graphics::Mesh> mesh
			= Resources::Find<graphics::Mesh>(name);

		mMesh = mesh;
	}

	void BaseRenderer::SetMaterial(const std::wstring& name)
	{
		std::shared_ptr<graphics::Material> material
			= Resources::Find<graphics::Material>(name);

		mMaterial = material;
	}
}
