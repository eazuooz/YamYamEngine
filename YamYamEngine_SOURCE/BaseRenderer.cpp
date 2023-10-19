#include "BaseRenderer.h"
#include "yaResources.h"
#include "yaMesh.h"


namespace ya
{
	BaseRenderer::BaseRenderer(eComponentType type)
		: Component(type)
	{
		//mMaterials.resize(1);
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
	void BaseRenderer::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		mMesh = mesh;

		std::vector<MeshData*> meshDatas = mesh->GetMeshDatas();

		for (MeshData* data : meshDatas)
		{
			for (MeshData::MaterialData matData : data->materials)
			{
				std::shared_ptr<graphics::Material> material
					= Resources::Find<graphics::Material>(matData.name);

				mMaterials.push_back(material);
			}
		}




		//mMaterials.resize(meshDatas[0]->materials.size());
		//for (size_t i = 0; i < mMaterials.size(); i++)
		//{
		//	mMaterials[i]->SetRenderingMode(graphics::eRenderingMode::Opaque);
		//}

		//int a = 0;

	}

	void BaseRenderer::SetMesh(const std::wstring& name)
	{
		std::shared_ptr<Mesh> mesh
			= Resources::Find<Mesh>(name);

		mMesh = mesh;

		std::vector<MeshData*> meshDatas = mesh->GetMeshDatas();

		for (MeshData* data : meshDatas)
		{
			for (MeshData::MaterialData matData : data->materials)
			{
				std::shared_ptr<graphics::Material> material = nullptr;
				if (matData.name == L"")
				{
					material = Resources::Find<graphics::Material>(L"PhongMaterial");
				}

				//std::shared_ptr<graphics::Material> material
				//	= Resources::Find<graphics::Material>(matData.name);

				


				//material->SetTexture(graphics::eTextureType::Albedo, texture);

				mMaterials.push_back(material);
			}
		}

	}

	void BaseRenderer::SetMaterial(const std::wstring& name)
	{
		std::shared_ptr<graphics::Material> material
			= Resources::Find<graphics::Material>(name);

		mMaterials.push_back(material);
	}
}
