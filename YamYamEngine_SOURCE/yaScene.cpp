#include "yaScene.h"
#include "yaGameObject.h"


namespace ya
{
    Scene::Scene(eSceneType type)
        : mType(type)
    {
    }

    Scene::~Scene()
    {

    }

    void Scene::Initialize()
    {
        for (Layer& layer : mLayers)
        {
            layer.Initialize();
        }
    }

    void Scene::Update()
    {
        for (Layer& layer : mLayers)
        {
            layer.Update();
        }
    }

    void Scene::FixedUpdate()
    {
        for (Layer& layer : mLayers)
        {
            layer.FixedUpdate();
        }
    }

    void Scene::Render()
    {
        for (Layer& layer : mLayers)
        {
            layer.Render();
        }
    }

    void Scene::Destroy()
    {
        for (Layer& layer : mLayers)
        {
            layer.Destroy();
        }


        // 메모리 삭제
    }

    void Scene::OnEnter()
    {
    }
    void Scene::OnExit()
    {
    }


    void Scene::AddGameObject(GameObject* gameObject, eLayerType layerIndex)
    {
        mLayers[(UINT)layerIndex].AddGameObject(gameObject);
        gameObject->SetLayerType(layerIndex);
    }

    std::vector<GameObject*> Scene::GetDontDestroyGameObjects()
    {
        std::vector<GameObject*> gameObjects;
        for (Layer& layer : mLayers)
        {
            std::vector<GameObject*> dontGameObjs
                = layer.GetDontDestroyGameObjects();

            gameObjects.insert(gameObjects.end()
                , dontGameObjs.begin()
                , dontGameObjs.end());
        }

        return gameObjects;
    }
}