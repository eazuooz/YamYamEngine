#pragma once
#include "yaComponent.h"
#include "yaMesh.h"
#include "yaShader.h"
#include "yaMaterial.h"

namespace ya
{
	class BaseRenderer : public Component
	{
	public:
		BaseRenderer(eComponentType type);
		~BaseRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void SetMesh(std::shared_ptr<graphics::Mesh> mesh);
		void SetMesh(const std::wstring& name);

		void SetMaterial(std::shared_ptr<graphics::Material> material, UINT index) { mMaterials[index] = material; }
		void SetMaterial(std::shared_ptr<graphics::Material> material) { mMaterials[0] = material; }

		void SetMaterial(const std::wstring& name);
		std::shared_ptr<graphics::Mesh> GetMesh() { return mMesh; }
		std::shared_ptr<graphics::Material> GetMaterial() { return mMaterials[0]; }
		std::vector<std::shared_ptr<graphics::Material>>& GetMaterials() { return mMaterials; }

	private:
		std::shared_ptr<graphics::Mesh> mMesh;
		std::vector<std::shared_ptr<graphics::Material>> mMaterials;
	};
}
