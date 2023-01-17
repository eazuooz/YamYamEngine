#include "yaSceneManager.h"
#include "yaResources.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaMeshRenderer.h"
#include "yaTexture.h"
#include "yaCamera.h"

namespace ya
{
	Scene* SceneManager::mPlayScene = nullptr;

	void SceneManager::Initialize()
	{
		mPlayScene = new Scene();

		GameObject* object = new GameObject();
		Transform* tr = new Transform();
		tr->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
		object->AddComponent(tr);

		
		MeshRenderer* meshRenderer = new MeshRenderer();
		
		//Test
		//meshRenderer->SetMesh(Resources::Find<Mesh>(L"TriangleMesh"));
		//meshRenderer->SetMaterial(Resources::Find<Material>(L"TriangleMaterial"));

		//SpriteDefaultMaterial
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"SpriteDefaultMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(L"SpriteDefaultMaterial"));

		object->AddComponent(meshRenderer);

		GameObject* camera = new GameObject();
		tr = new Transform();
		tr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		camera->AddComponent(tr);

		Camera* cameraComp = new Camera();
		camera->AddComponent(cameraComp);

		mPlayScene->AddGameObject(object, eLayer::None);
		mPlayScene->AddGameObject(camera, eLayer::None);

		//std::shared_ptr<Texture> texture = Resources::Load<Texture>(L"TriangleTexture", L"..\\Resources\\Triangle.png");
		//texture->BindShader(eShaderStage::PS, 0);
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
