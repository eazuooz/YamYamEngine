#include "yaSceneManager.h"
#include "yaResources.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaMeshRenderer.h"
#include "yaTexture.h"
#include "yaCamera.h"
#include "yaGridScript.h"
#include "yaCollider2D.h"
#include "yaPlayer.h"
#include "CollisionManager.h"

namespace ya
{
	Scene* SceneManager::mActiveScene = nullptr;

	void SceneManager::Initialize()
	{
		mActiveScene = new Scene();

		//camera
		GameObject* camera = new GameObject();
		Transform* cameraTr = camera->GetComponent<Transform>();
		cameraTr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		Camera* cameraComp = camera->AddComponent<Camera>();
		mActiveScene->AddGameObject(camera, eLayerType::None);
		renderer::mainCamera = cameraComp;


		//parent
		GameObject* parent = new GameObject();
		Transform* parentTr = parent->GetComponent<Transform>();
		Collider2D* collider = parent->AddComponent<Collider2D>();

		parentTr->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
		parentTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f/*1.5708f*/));

		collider->SetType(eColliderType::Rect);

		MeshRenderer* meshRenderer = parent->AddComponent<MeshRenderer>();
		parent->AddComponent<Player>();
		//SpriteDefaultMaterial
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"SpriteDefaultMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(L"SpriteDefaultMaterial"));
		
		mActiveScene->AddGameObject(parent, eLayerType::None);

		//child
		GameObject* child = new GameObject();
		Transform* childTr = child->GetComponent<Transform>();
		childTr->SetPosition(Vector3(2.0f, 0.0f, 19.0f));
		childTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
		Collider2D* childCollider = child->AddComponent<Collider2D>();
		//childTr->SetParent(parentTr);

		meshRenderer = child->AddComponent<MeshRenderer>();
		//meshRenderer->SetMesh(Resources::Find<Mesh>(L"TriangleMesh"));
		//meshRenderer->SetMaterial(Resources::Find<Material>(L"TriangleMaterial"));

		//SpriteDefaultMaterial
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"SpriteDefaultMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(L"SpriteDefaultMaterial"));

		mActiveScene->AddGameObject(child, eLayerType::None);

		CollisionManager::CollisionLayerCheck((UINT)eLayerType::None, (UINT)eLayerType::None, true);

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
