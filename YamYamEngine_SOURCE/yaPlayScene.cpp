#include "yaPlayScene.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaMeshRenderer.h"
#include "yaTexture.h"
#include "yaCamera.h"
#include "yaGridScript.h"
#include "yaCollider2D.h"
#include "yaPlayer.h"
#include "CollisionManager.h"
#include "yaSpriteRenderer.h"
#include "yaAnimator.h"
#include "yaCameraScript.h"
#include "yaResources.h"
#include "yaObject.h"
#include "yaInput.h"
#include "yaLight.h"

namespace ya
{



	PlayScene::PlayScene()
		: Scene(eSceneType::Play)
	{

	}

	PlayScene::~PlayScene()
	{
	}

	void PlayScene::Initialize()
	{
		// Main Camera Game Object
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::None, this);
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		//cameraComp->RegisterCameraInRenderer();
		//cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<CameraScript>();
		//renderer::cameras[0] = cameraComp;

		//Direction Light
		{
			GameObject* directionLight = object::Instantiate<GameObject>(eLayerType::None, this);
			Light* light = directionLight->AddComponent<Light>();

			light->SetDiffuse(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
			light->SetAmbient(Vector4(0.2f, 0.2f, 0.2f, 1.0f));
			light->SetType(eLightType::Directional);
		}
		//Point Light
		{
			GameObject* directionLight = object::Instantiate<GameObject>(eLayerType::None, this);
			directionLight->GetComponent<Transform>()->SetPosition(Vector3(3.0f, 0.0f, 0.0f));
			Light* light = directionLight->AddComponent<Light>();
			light->SetDiffuse(Vector4(1.0f, 0.0f, 1.0f, 1.0f));
			light->SetRadius(300.0f);
			light->SetType(eLightType::Point);
		}



		//parent
		GameObject* parent = new GameObject();
		Transform* parentTr = parent->GetComponent<Transform>();
		Collider2D* collider = parent->AddComponent<Collider2D>();

		parentTr->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
		parentTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f/*1.5708f*/));

		collider->SetType(eColliderType::Rect);
		object::DontDestroyOnLoad(parent);

		SpriteRenderer* meshRenderer = parent->AddComponent<SpriteRenderer>();
		Animator* animator = parent->AddComponent<Animator>();
		std::shared_ptr<Texture> linkTex = Resources::Load<Texture>(L"Link", L"..\\Resources\\link.png");
		animator->CreateAnimation(L"Link", linkTex, Vector2(0.0f, 650.0f), Vector2(120.0f, 130.0f), Vector2::Zero, 120.0f, 10, 0.1f);
		animator->Play(L"Link", true);
		//MeshRenderer* meshRenderer = parent->AddComponent<MeshRenderer>();
		parent->AddComponent<Player>();
		////SpriteDefaultMaterial
		//meshRenderer->SetMesh(Resources::Find<Mesh>(L"SpriteDefaultMesh"));
		//meshRenderer->SetMaterial(Resources::Find<Material>(L"SpriteDefaultMaterial"));

		AddGameObject(parent, eLayerType::None);

		//child
		GameObject* child = new GameObject();
		Transform* childTr = child->GetComponent<Transform>();
		childTr->SetPosition(Vector3(2.0f, 0.0f, 19.0f));
		childTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
		Collider2D* childCollider = child->AddComponent<Collider2D>();
		//childTr->SetParent(parentTr);

		meshRenderer = child->AddComponent<SpriteRenderer>();
		//meshRenderer->SetMesh(Resources::Find<Mesh>(L"TriangleMesh"));
		//meshRenderer->SetMaterial(Resources::Find<Material>(L"TriangleMaterial"));

		//SpriteDefaultMaterial
		//meshRenderer->SetMesh(Resources::Find<Mesh>(L"SpriteDefaultMesh"));
		//meshRenderer->SetMaterial(Resources::Find<Material>(L"SpriteDefaultMaterial"));

		AddGameObject(child, eLayerType::None);

		CollisionManager::CollisionLayerCheck((UINT)eLayerType::None, (UINT)eLayerType::None, true);

		Scene::Initialize();
	}

	void PlayScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(eSceneType::Tilte);
		}

		Scene::Update();
	}

	void PlayScene::FixedUpdate()
	{
		Scene::FixedUpdate();
	}

	void PlayScene::Render()
	{
		Scene::Render();
	}

	void PlayScene::OnEnter()
	{
	}

	void PlayScene::OnExit()
	{
	}

}
