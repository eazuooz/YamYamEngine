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
		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -10.0f));

		// Environment sphere 
		{
			GameObject* sphere = object::Instantiate<GameObject>(eLayerType::Player, this);
			sphere->SetName(L"Sphere");

			sphere->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

			MeshRenderer* mr = sphere->AddComponent<MeshRenderer>();
			std::shared_ptr<Material> mt = Resources::Find<Material>(L"EnvMaterial");
			std::shared_ptr<Texture> tex = Resources::Find<Texture>(L"SpriteDefaultTexture");
			std::shared_ptr<Mesh>  mesh = Resources::Find<Mesh>(L"SphereMesh");

			mt->SetTexture(eTextureType::Albedo, tex);
			mr->ResizeMaterial(1);
			mr->SetMaterial(mt);
			mr->SetMesh(mesh);
		}

		//zelda
		{
			GameObject* player = object::Instantiate<GameObject>(eLayerType::Player, this);
			player->GetComponent<Transform>()->SetPosition(Vector3(3.0f, 0.0f, 0.0f));
			player->GetComponent<Transform>()->SetScale(Vector3(2.5f, 2.5f, 2.5f));
			MeshRenderer* mr = player->AddComponent<MeshRenderer>();
			mr->SetMesh(L"Zelda");
		}
		//
		//monster
		//{	
		//	GameObject* monster = object::Instantiate<GameObject>(eLayerType::Player, this);
		//	monster->GetComponent<Transform>()->SetPosition(Vector3(-3.0f, -1.0f, 0.0f));
		//	monster->GetComponent<Transform>()->SetScale(Vector3(0.1f, 0.1f, 0.1f));

		//	SkinnedMeshRenderer* mr = monster->AddComponent<SkinnedMeshRenderer>();
		//	Animator* animator = monster->AddComponent<Animator>();
		//	std::shared_ptr<Mesh> house = std::make_shared<Mesh>();
		//	house->LoadFromFbx(L"Monster.fbx", animator);
		//	mr->SetMesh(house);
		//	animator->Play(L"Take 001", true);
		//}

		//Direction Light
		{
			GameObject* directionLight = object::Instantiate<GameObject>(eLayerType::None, this);
			directionLight->SetName(L"DirectionLight");
			directionLight->GetComponent<Transform>()->SetPosition(Vector3(30.0f, 30.0f, 0.0f));
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
			directionLight->GetComponent<Transform>()->SetPosition(Vector3(-3.0f, 0.0f, -0.5f));
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
			directionLight->GetComponent<Transform>()->SetPosition(Vector3(3.0f, 0.0f, -2.0f));
			directionLight->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 45.0f, 0.0f));

			Light* light = directionLight->AddComponent<Light>();
			light->SetColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
			light->SetRadius(10.0f);
			light->SetPower(1.0f);
			light->SetType(eLightType::Spot);
		}


		// cube map
		{
			GameObject* cubemap = object::Instantiate<GameObject>(eLayerType::None, this);
			cubemap->SetName(L"CubeMap");

			MeshRenderer* mr = cubemap->AddComponent<MeshRenderer>();
			std::shared_ptr<Material> mt = Resources::Find<Material>(L"CubeMapMaterial");
			std::shared_ptr<Texture> tex = Resources::Find<Texture>(L"SpecularCube");
			std::shared_ptr<Mesh>  mesh = Resources::Find<Mesh>(L"CubeMesh");
			
			mt->SetTexture(eTextureType::SpecularCube, tex);
			tex = Resources::Find<Texture>(L"DiffuseCube");
			mt->SetTexture(eTextureType::DiffuseCube, tex);

			mr->ResizeMaterial(1);
			mr->SetMaterial(mt);
			mr->SetMesh(mesh);
		}

		//PostProcess
		{
			GameObject* post = object::Instantiate<GameObject>(eLayerType::None, this);
			post->SetName(L"CubeMap");
			post->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -9.0f));
			post->GetComponent<Transform>()->SetScale(Vector3(2.2f, 1.2f, 1.0f));
			
			MeshRenderer* mr = post->AddComponent<MeshRenderer>();
			std::shared_ptr<Material> mt = Resources::Find<Material>(L"PostProcessMaterial");
			std::shared_ptr<Mesh>  mesh = Resources::Find<Mesh>(L"RectMesh");

			mr->ResizeMaterial(1);
			mr->SetMaterial(mt);
			mr->SetMesh(mesh);
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
