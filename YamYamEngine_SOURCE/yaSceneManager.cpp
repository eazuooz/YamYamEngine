#include "yaSceneManager.h"
#include "yaResources.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaMeshRenderer.h"
#include "yaTexture.h"
#include "yaCamera.h"

namespace ya
{
	Scene* SceneManager::mActiveScene = nullptr;

	void SceneManager::Initialize()
	{
		mActiveScene = new Scene();

		GameObject* object = new GameObject();
		Transform* tr = new Transform();
		tr->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
		tr->SetRotation(Vector3(0.0f, 0.0f, 1.5708f));
		object->AddComponent(tr);

		
		MeshRenderer* meshRenderer = new MeshRenderer();
		
		//Test
		//meshRenderer->SetMesh(Resources::Find<Mesh>(L"TriangleMesh"));
		//meshRenderer->SetMaterial(Resources::Find<Material>(L"TriangleMaterial"));

		//SpriteDefaultMaterial
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"SpriteDefaultMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(L"SpriteDefaultMaterial"));

		object->AddComponent(meshRenderer);
		mActiveScene->AddGameObject(object, eLayer::None);

		//object = new GameObject();
		//tr = new Transform();
		//tr->SetPosition(Vector3(0.5f, 0.0f, 21.0f));
		//object->AddComponent(tr);

		//meshRenderer = new MeshRenderer();
		//meshRenderer->SetMesh(Resources::Find<Mesh>(L"SpriteDefaultMesh"));
		//meshRenderer->SetMaterial(Resources::Find<Material>(L"SpriteDefaultMaterial"));

		//object->AddComponent(meshRenderer);
		//mPlayScene->AddGameObject(object, eLayer::None);

		GameObject* camera = new GameObject();
		tr = new Transform();
		tr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		camera->AddComponent(tr);

		Camera* cameraComp = new Camera();
		camera->AddComponent(cameraComp);

		
		mActiveScene->AddGameObject(camera, eLayer::None);

		//std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"TriangleTexture", L"..\\Resources\\Triangle.png");
		//texture->BindShader(eShaderStage::PS, 0);

		mActiveScene->Initialize();
	}

	void SceneManager::Update()
	{
		mActiveScene->Update();
	}

	void SceneManager::FixedUpdate()
	{
		mActiveScene->FixedUpdate();
	}

	void SceneManager::Render()
	{
		mActiveScene->Render();
	}
	
	void SceneManager::Release()
	{
		delete mActiveScene;
		mActiveScene = nullptr;
	}
}
