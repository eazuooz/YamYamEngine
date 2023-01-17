#pragma once
#include "yaComponent.h"
#include "yaMesh.h"
#include "yaShader.h"
#include "yaMaterial.h"

namespace ya
{
	using namespace graphics;

	class SpriteRenderer : public Component
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetMesh(std::shared_ptr<Mesh> mesh) { mMesh = mesh; }
		void SetMaterial(std::shared_ptr <Material> material) { mMaterial = material; }

	private:
		void InitializeResource();

	private:
		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr <Material> mMaterial;
	};
}