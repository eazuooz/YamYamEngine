#include "yaScene.h"
#include "yaGameObject.h"


namespace ya
{
    Scene::Scene()
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

    void Scene::AddGameObject(GameObject* gameObject, eLayerType layerIndex)
    {
        mLayers[(UINT)layerIndex].AddGameObject(gameObject);
    }
}