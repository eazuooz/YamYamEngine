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
#include "yaPaintShader.h"
#include "ParticleSystem.h"
#include "yaAudioListener.h"
#include "yaAudioSource.h"
#include "yaAudioClip.h"
#include "yaFbxLoader.h"
#include "yaResources.h"
#include "yaSkinnedMeshRenderer.h"
#include "yaAnimator.h"

// 3D
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
		// main Camera 
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::None, this);
		cameraObj->SetName(L"MainCamera");

		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		renderer::mainCamera = cameraComp;
		cameraObj->AddComponent<CameraScript>();
		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 20.0f, -100.0f));

		// player
		GameObject* player = object::Instantiate<GameObject>(eLayerType::Player, this);
		player->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		//player->GetComponent<Transform>()->SetScale(Vector3(0.1f, 0.1f, 0.1f));
		//player->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 45.0f, 0.0f));

		//std::shared_ptr<Texture> texture
		//	= Resources::Load<Texture>(L"crate2_diffuse", L"..\\Resources\\crate2_diffuse.png");
		//std::shared_ptr<Material> material
		//	= Resources::Find<Material>(L"PhongMaterial");
		//material->SetTexture(eTextureType::Albedo, texture);

		SkinnedMeshRenderer* mr = player->AddComponent<SkinnedMeshRenderer>();
		Animator* animator = player->AddComponent<Animator>();
		
		//mr->SetMaterial(material);
		//mr->SetMesh(L"SphereMesh");
		//mr->SetMesh(L"Zelda");

		player->AddComponent<Player>();
		//cameraObj->AddComponent<CameraScript>();

		//FbxLoader::Load(L"zeldaPosed001.fbx");

		std::shared_ptr<Mesh> house = std::make_shared<Mesh>();
		//house->LoadFromFbx(L"House.fbx");
		Resources::Insert<Mesh>(L"House.fbx", house);
		//house->LoadFromFbx(L"c1020.fbx");
		house->LoadFromFbx(L"Monster.fbx", animator);
		//c1020.fbx
		mr->SetMesh(house);
		animator->Play(L"Take 001", true);

		//mr->SetMesh(L"Zelda");
		//FbxLoader::Load(L"House.fbx");

		//Direction Light
		{
			GameObject* directionLight = object::Instantiate<GameObject>(eLayerType::None, this);
			directionLight->SetName(L"DirectionLight");
			directionLight->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 30.0f, -30.0f));
			directionLight->GetComponent<Transform>()->SetRotation(Vector3(50.0f, -30.0f, 0.0f));

			Light* light = directionLight->AddComponent<Light>();
			light->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			light->SetPower(1.0f);
			light->SetType(eLightType::Directional);
		}
		//Point Light
		{
			GameObject* directionLight = object::Instantiate<GameObject>(eLayerType::None, this);
			directionLight->SetName(L"PointLight");
			directionLight->GetComponent<Transform>()->SetPosition(Vector3(2.0f, 0.0f, -2.0f));
			directionLight->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 0.0f, 0.0f));

			Light* light = directionLight->AddComponent<Light>();
			light->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
			light->SetRadius(10.0f);
			light->SetPower(1.0f);
			light->SetType(eLightType::Point);
		}
		//Spot Light
		{
			GameObject* directionLight = object::Instantiate<GameObject>(eLayerType::None, this);
			directionLight->SetName(L"SpotLight");
			directionLight->GetComponent<Transform>()->SetPosition(Vector3(-2.0f, 0.0f, -2.0f));
			directionLight->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 45.0f, 0.0f));

			Light* light = directionLight->AddComponent<Light>();
			light->SetColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
			light->SetRadius(3.0f);
			light->SetPower(1.0f);
			light->SetType(eLightType::Spot);
		}
		Scene::Initialize();
	}

	void PlayScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(L"TitleScene");
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
