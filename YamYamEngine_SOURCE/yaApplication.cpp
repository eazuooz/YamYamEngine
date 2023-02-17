#include "yaApplication.h"
#include "yaGraphicsDevice_DX11.h"

#include "yaInput.h"
#include "yaTime.h"
#include "yaRenderer.h"
#include "yaSceneManager.h"
#include "yaResources.h"

using namespace ya::graphics;

namespace ya
{
	Application::~Application()
	{

	}

	void Application::Run()
	{
		Update();
		FixedUpdate();
		Render();
	}

	void Application::Initialize()
	{
		Time::Initialize();
		Input::Initialize();

		renderer::Initialize();
		SceneManager::Initialize();
	}

	void Application::Update()
	{
		Time::Update();
		Input::Update();

		SceneManager::Update();
	}

	void Application::FixedUpdate()
	{
		SceneManager::FixedUpdate();
	}

	void Application::Render()
	{
		graphicsDevice->Clear();
		graphicsDevice->AdjustViewport();

		Time::Render();
		//SceneManager::Render();
		renderer::Render();

		//graphicsDevice->Render();
		
	}

	void Application::Present()
	{
		graphicsDevice->Present();
	}

	void Application::SetWindow(HWND hwnd, UINT width, UINT height)
	{
		if (graphicsDevice == nullptr)
		{
			mHwnd = hwnd;
			mWidth = width;
			mHegith = height;

			graphicsDevice = std::make_unique<GraphicsDevice_DX11>();
			ya::graphics::GetDevice() = graphicsDevice.get();
			//ya::device = graphicsDevice.get();
		}

		RECT rt = { 0, 0, (LONG)width , (LONG)height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(mHwnd, true);
		UpdateWindow(mHwnd);
	}

	void Application::Release()
	{
		SceneManager::Release();
		renderer::Release();
		Resources::Release();
	}
}
