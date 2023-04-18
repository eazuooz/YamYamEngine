#pragma once
#include "guiComponent.h"
#include "yaMaterial.h"
#include "yaMesh.h"

namespace gui
{
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		~MeshRenderer();

		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		std::shared_ptr<ya::graphics::Material> mMaterial;
		std::shared_ptr<ya::Mesh> mMesh;
	
	};
}
