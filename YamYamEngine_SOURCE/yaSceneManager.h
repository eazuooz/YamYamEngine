#pragma once
#include "yaScene.h"

namespace ya
{
	class SceneManager
	{
	public:
		template <typename T>
		static bool CreateScene(const std::wstring& name)
		{
			T* scene = new T();
			mScenes.insert(std::make_pair(name, scene));
			scene->Initialize();

			return scene;
		}

		static void Initialize();
		static void Update();
		static void FixedUpdate();
		static void Destroy();
		static void Release();

		static Scene* LoadScene(const std::wstring& name);
		static Scene* GetActiveScene() { return mActiveScene; }

	private:
		static std::map<std::wstring, Scene*> mScenes;
		static Scene* mActiveScene;
	};
}
