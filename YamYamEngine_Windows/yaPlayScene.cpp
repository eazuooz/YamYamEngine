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
		// Main Camera 
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::None, this);
		cameraObj->SetName(L"MainCamera");

		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		renderer::mainCamera = cameraComp;
		cameraObj->AddComponent<CameraScript>();
		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -10.0f));

		//Direction Light
		//{
		//	GameObject* directionLight = object::Instantiate<GameObject>(eLayerType::None, this);
		//	directionLight->SetName(L"DirectionLight");
		//	Light* light = directionLight->AddComponent<Light>();

		//	light->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		//	light->SetType(eLightType::Directional);
		//}
		////Point Light
		//{
		//	GameObject* directionLight = object::Instantiate<GameObject>(eLayerType::None, this);
		//	directionLight->SetName(L"PointLight");
		//	directionLight->GetComponent<Transform>()->SetPosition(Vector3(3.0f, 0.0f, 0.0f));
		//	Light* light = directionLight->AddComponent<Light>();
		//	light->SetColor(Vector4(1.0f, 0.0f, 1.0f, 1.0f));
		//	light->SetRadius(300.0f);
		//	light->SetType(eLightType::Point);
		//}

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
