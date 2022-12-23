#include "yaScene.h"



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

    void Scene::AddGameObject(GameObject* gameObject, eLayer layerIndex)
    {
        mLayers[(UINT)layerIndex].AddGameObject(gameObject);
    }
}