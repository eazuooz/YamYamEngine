#pragma once
#include "yaComponent.h"
#include "yaMesh.h"
#include "yaShader.h"
#include "yaMaterial.h"

namespace ya
{
	using namespace graphics;

	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		~MeshRenderer();
		
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetMesh(Mesh* mesh) { mMesh = mesh; }
		void SetMaterial(Material* material) { mMaterial = material; }


	private:
		Mesh* mMesh;
		Material* mMaterial;
	};
}
