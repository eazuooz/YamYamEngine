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
#include "yaSpriteRenderer.h"
#include "yaAnimator.h"
#include "yaCameraScript.h"
#include "yaPlayScene.h"
#include "yaTitleScene.h"

namespace ya
{
	Scene* SceneManager::mActiveScene = nullptr;
	std::vector<Scene*> SceneManager::mScenes = {};
	void SceneManager::Initialize()
	{
		mScenes.resize((UINT)eSceneType::End);
		mScenes[(UINT)eSceneType::Tilte] = new TitleScene();
		mScenes[(UINT)eSceneType::Play] = new PlayScene();
		
		mActiveScene = mScenes[(UINT)eSceneType::Play];

		
		for (Scene* scene : mScenes)
		{
			if (scene == nullptr)
				continue;

			scene->Initialize();
		}
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
		for (Scene* scene : mScenes)
		{
			delete scene;
			scene = nullptr;
		}
	}

	void SceneManager::LoadScene(eSceneType type)
	{
		if (mActiveScene)
			mActiveScene->OnExit();

		// 바뀔때 dontDestory 오브젝트는 다음씬으로 같이 넘겨줘야한다.
		std::vector<GameObject*> gameObjs
			= mActiveScene->GetDontDestroyGameObjects();
		mActiveScene = mScenes[(UINT)type];

		for (GameObject* obj : gameObjs)
		{
			eLayerType type = obj->GetLayerType();
			mActiveScene->AddGameObject(obj, type);
		}

		mActiveScene->OnEnter();
	}
}
