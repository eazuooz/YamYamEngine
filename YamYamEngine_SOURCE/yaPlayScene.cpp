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
		//L"..\\Resources\\DefaultSprite.png");
		

		//cs shader
		std::shared_ptr<PaintShader> paint = Resources::Find<PaintShader>(L"PaintShader");
		paint->SetTexture(Resources::Find<Texture>(L"UAVTexture"));
		paint->OnExcute();
		
		// Main Camera Game Object
		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::None, this);
		cameraObj->SetName(L"MainCamera");
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		renderer::mainCamera = cameraComp;
		cameraObj->AddComponent<CameraScript>();
		cameraObj->AddComponent<AudioListener>();

		//Direction Light
		{
			GameObject* directionLight = object::Instantiate<GameObject>(eLayerType::None, this);
			directionLight->SetName(L"DirectionLight");
			Light* light = directionLight->AddComponent<Light>();

			light->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			light->SetAmbient(Vector4(0.2f, 0.2f, 0.2f, 1.0f));
			light->SetType(eLightType::Directional);
		}
		//Point Light
		{
			GameObject* directionLight = object::Instantiate<GameObject>(eLayerType::None, this);
			directionLight->SetName(L"PointLight");
			directionLight->GetComponent<Transform>()->SetPosition(Vector3(3.0f, 0.0f, 0.0f));
			Light* light = directionLight->AddComponent<Light>();
			light->SetDiffuse(Vector4(1.0f, 0.0f, 1.0f, 1.0f));
			light->SetRadius(300.0f);
			light->SetType(eLightType::Point);
		}



		//parent
		GameObject* parent = new GameObject();
		parent->SetName(L"Zelda");
		renderer::inspectorGameObject = parent;
		Transform* parentTr = parent->GetComponent<Transform>();
		Collider2D* collider = parent->AddComponent<Collider2D>();

		parentTr->SetPosition(Vector3(0.0f, 0.0f, 2.0f));
		parentTr->SetRotation(Vector3(0.0f, 0.0f, 0.0f/*1.5708f*/));
		parentTr->SetScale(Vector3(200.0f, 200.0f, 1.0f));
		
		std::shared_ptr<AudioClip> audioClip 
			= Resources::Load<AudioClip>(L"BGSound", L"..\\Resources\\Sound\\smw_bonus_game_end.wav");
		
		AudioSource* audioSource = parent->AddComponent<AudioSource>();
		audioSource->SetClip(audioClip);
		audioSource->SetLoop(true);
		audioSource->Play();

		collider->SetType(eColliderType::Rect);
		object::DontDestroyOnLoad(parent);

		MeshRenderer* meshRenderer = parent->AddComponent<MeshRenderer>();
		Animator* animator = parent->AddComponent<Animator>();
		std::shared_ptr<Texture> linkTex = Resources::Load<Texture>(L"Link", L"..\\Resources\\link.png");
		animator->CreateAnimation(L"Link", linkTex, Vector2(0.0f, 650.0f), Vector2(120.0f, 130.0f), Vector2::Zero, 120.0f, 10, 0.1f);
		animator->Play(L"Link", true);
		//MeshRenderer* meshRenderer = parent->AddComponent<MeshRenderer>();
		parent->AddComponent<Player>();
		////SpriteDefaultMaterial
		meshRenderer->SetMesh(Resources::Find<Mesh>(L"SpriteDefaultMesh"));
		meshRenderer->SetMaterial(Resources::Find<Material>(L"SpriteDefaultMaterial"));

		AddGameObject(parent, eLayerType::None);

		//child
		GameObject* child = new GameObject();
		child->SetName(L"Child");
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



		AddGameObject(child, eLayerType::None);

		// particle 
		GameObject* particle = object::Instantiate<GameObject>(eLayerType::None, this);
		particle->SetName(L"Particle");
		Transform* particleTr = particle->GetComponent<Transform>();
		particleTr->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
		ParticleSystem* particleSystem = particle->AddComponent<ParticleSystem>();


		//post process object
		GameObject* postProcess = object::Instantiate<GameObject>(eLayerType::None, this);
		postProcess->SetName(L"PostProcess");
		Transform* postProcessTr = postProcess->GetComponent<Transform>();
		postProcessTr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		postProcessTr->SetScale(Vector3(200.0f, 200.0f, 1.0f));

		SpriteRenderer* postRenderer = postProcess->AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> postMtr = Resources::Find<Material>(L"PostProcessMaterial");
		postRenderer->SetMaterial(postMtr);



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
