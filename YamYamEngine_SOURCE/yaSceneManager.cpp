#include "yaSceneManager.h"
#include "yaResources.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaMeshRenderer.h"
#include "yaTexture.h"
#include "yaCamera.h"
#include "yaGridScript.h"
#include "yaCollider2D.h"

namespace ya
{
	Scene* SceneManager::mActiveScene = nullptr;

	void SceneManager::Initialize()
	{
		mActiveScene = new Scene();

		//camera
		GameObject* camera = new GameObject();
		Transform* cameraTr = new Transform();
		cameraTr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		camera->AddComponent(cameraTr);
		Camera* cameraComp = new Camera();
		camera->AddComponent(cameraComp);
		mActiveScene->AddGameObject(camera, eLayer::None);
		renderer::mainCamera = cameraComp;


		//parent
		GameObject* parent = new GameObject();
		Transform* parentTr = new Transform();
		Collider2D* collider = new Collider2D();

		parentTr->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
		parentTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f/*1.5708f*/));
		parent->AddComponent(parentTr);

		collider->SetType(eColliderType::Rect);
		parent->AddComponent(collider);

		MeshRenderer* meshRenderer = new MeshRenderer();
		//meshRenderer->SetMesh(Resources::Find<Mesh>(L"TriangleMesh"));
		//meshRenderer->SetMaterial(Resources::Find<Material>(L"TriangleMaterial"));

		//SpriteDefaultMaterial
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"SpriteDefaultMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(L"SpriteDefaultMaterial"));

		parent->AddComponent(meshRenderer);
		mActiveScene->AddGameObject(parent, eLayer::None);

		//child
		GameObject* child = new GameObject();
		Transform* childTr = new Transform();
		childTr->SetPosition(Vector3(3.0f, 0.0f, 0.0f));
		childTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
		child->AddComponent(childTr);
		childTr->SetParent(parentTr);

		meshRenderer = new MeshRenderer();
		//meshRenderer->SetMesh(Resources::Find<Mesh>(L"TriangleMesh"));
		//meshRenderer->SetMaterial(Resources::Find<Material>(L"TriangleMaterial"));

		//SpriteDefaultMaterial
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"SpriteDefaultMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(L"SpriteDefaultMaterial"));

		child->AddComponent(meshRenderer);
		mActiveScene->AddGameObject(child, eLayer::None);




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

	void SceneManager::Destroy()
	{
		mActiveScene->Destroy();
	}

	//void SceneManager::Render()
	//{
	//	mActiveScene->Render();
	//}
	
	void SceneManager::Release()
	{
		delete mActiveScene;
		mActiveScene = nullptr;
	}
}
