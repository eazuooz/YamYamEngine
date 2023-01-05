#include "yaSceneManager.h"
#include "yaResources.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaMeshRenderer.h"
#include "yaTexture.h"

namespace ya
{
	Scene* SceneManager::mPlayScene = nullptr;

	void SceneManager::Initialize()
	{
		mPlayScene = new Scene();

		GameObject* object = new GameObject();
		Transform* tr = new Transform();
		tr->SetPosition(Vector3(1.0f, 0.0f, 0.0f));
		object->AddComponent(tr);

		MeshRenderer* meshRenderer = new MeshRenderer();
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"TriangleMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(L"TriangleMaterial"));
		
		
		object->AddComponent(meshRenderer);

		std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"TriangleTexture", L"..\\Resources\\Triangle.png");
		texture->BindShader(eShaderStage::PS, 0);



		mPlayScene->AddGameObject(object, eLayer::None);
	}

	void SceneManager::Update()
	{
		mPlayScene->Update();
	}

	void SceneManager::FixedUpdate()
	{
		mPlayScene->FixedUpdate();
	}

	void SceneManager::Render()
	{
		mPlayScene->Render();
	}
	
	void SceneManager::Release()
	{
		delete mPlayScene;
		mPlayScene = nullptr;
	}
}
