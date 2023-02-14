#pragma once
#include "yaScene.h"
#include "yaLayer.h"
#include "yaGameObject.h"
#include "yaSceneManager.h"

namespace ya::gameObject
{
	template <typename T>
	static __forceinline T* Instantiate(enums::eLayer layer)
	{
		T* gameObject = new T();
		Scene* scene = SceneManager::GetActiveScene();
		Layer* layer = scene->GetLayer();
		layer->AddGameObject(dynamic_cast<GameObject*>(gameObject), layer);

		return gameObject;
	}

	template <typename T>
	static __forceinline T* Instantiate(Vector2 position, enums::eLayer type)
	{
		T* gameObject = new T(position);
		Scene* scene = SceneManager::GetActiveScene();
		Layer* layer = scene->GetLayer();
		layer->AddGameObject(dynamic_cast<GameObject*>(gameObject), type);

		return gameObject;
	}

	template <typename T>
	static __forceinline T* Instantiate(Vector3 position, enums::eLayer type)
	{
		T* gameObject = new T(position);
		Scene* scene = SceneManager::GetActiveScene();
		Layer* layer = scene->GetLayer();
		layer->AddGameObject(dynamic_cast<GameObject*>(gameObject), type);

		return gameObject;
	}

	static __forceinline void DontDestroyOnLoad(GameObject* gameObject)
	{

	}
}