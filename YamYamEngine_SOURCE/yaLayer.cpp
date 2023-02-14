#include "yaLayer.h"


namespace ya
{
	Layer::Layer()
	{
	}
	Layer::~Layer()
	{
		for (GameObject* gameObject : mGameObjects)
		{
			delete gameObject;
			gameObject = nullptr;
		}
	}

	void Layer::Initialize()
	{
		for (GameObject* gameObject : mGameObjects)
		{
			gameObject->Initialize();
		}
	}

	void Layer::Update()
	{
		for (GameObject* gameObject : mGameObjects)
		{
			gameObject->Update();
		}
	}

	void Layer::FixedUpdate()
	{
		for (GameObject* gameObject : mGameObjects)
		{
			gameObject->FixedUpdate();
		}
	}

	void Layer::Render()
	{
		for (GameObject* gameObject : mGameObjects)
		{
			gameObject->Render();
		}
	}

	void Layer::Destroy()
	{
		// find death gameObject
		std::set<GameObject*> deleteGameObjs;
		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj->IsDead())
				deleteGameObjs.insert(gameObj);
		}

		// erase gameobject in layer
		typedef std::vector<GameObject*>::iterator GameObjectsIter;
		for (GameObjectsIter iter = mGameObjects.begin()
			; iter != mGameObjects.end()
			; )
		{
			std::set<GameObject*>::iterator deleteIter
				= deleteGameObjs.find((*iter));

			if (deleteIter != deleteGameObjs.end())
			{
				mGameObjects.erase(iter);
				continue;
			}
			
			iter++;
		}

		// delete death object
		for ( GameObject* iter : deleteGameObjs )
		{
			delete iter;
			iter = nullptr;
		}
	}

	void Layer::AddGameObject(GameObject* gameObject)
	{
		mGameObjects.push_back(gameObject);
	}
}