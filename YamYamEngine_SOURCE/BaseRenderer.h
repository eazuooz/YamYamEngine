#pragma once
#include "yaComponent.h"
#include "yaMesh.h"
#include "yaShader.h"
#include "yaMaterial.h"

namespace ya
{
	using namespace graphics;
	class BaseRenderer : public Component
	{
	public:
		BaseRenderer(eComponentType type);
		~BaseRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void SetMesh(std::shared_ptr<Mesh> mesh) { mMesh = mesh; }
		void SetMesh(const std::wstring& name);
		void SetMaterial(std::shared_ptr <Material> material) { mMaterial = material; }
		void SetMaterial(const std::wstring& name);
		std::shared_ptr<Mesh> GetMesh() { return mMesh; }
		std::shared_ptr <Material> GetMaterial() { return mMaterial; }

	private:
		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr <Material> mMaterial;
	};
}
