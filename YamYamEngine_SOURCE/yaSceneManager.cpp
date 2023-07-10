#include "yaSceneManager.h"
#include "yaResources.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaMeshRenderer.h"
#include "yaTexture.h"
#include "yaCamera.h"
#include "yaCollider2D.h"
#include "CollisionManager.h"
#include "yaSpriteRenderer.h"
#include "yaAnimator.h"

namespace ya
{
	Scene* SceneManager::mActiveScene = nullptr;
	std::map<std::wstring, Scene*> SceneManager::mScenes = {};
	void SceneManager::Initialize()
	{

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

	void SceneManager::Release()
	{
		for (auto Iter : mScenes)
		{
			delete Iter.second;
			Iter.second = nullptr;
		}
	}

	Scene* SceneManager::LoadScene(const std::wstring& name)
	{
		if (mActiveScene)
			mActiveScene->OnExit();

		std::vector<GameObject*> gameObjs = {};
		if (mActiveScene)
			gameObjs = mActiveScene->GetDontDestroyGameObjects();
		

		auto iter = mScenes.find(name);
		if (iter == mScenes.end())
			return nullptr;

		mActiveScene = iter->second;

		for (GameObject* obj : gameObjs)
		{
			eLayerType type = obj->GetLayerType();
			mActiveScene->AddGameObject(obj, type);
		}

		mActiveScene->OnEnter();
	}
}
