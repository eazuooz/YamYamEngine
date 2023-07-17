#include "yaApplication.h"
#include "yaGraphicsDevice_DX11.h"

#include "yaInput.h"
#include "yaTime.h"
#include "yaRenderer.h"
#include "yaSceneManager.h"
#include "yaResources.h"
#include "CollisionManager.h"
#include "yaFmod.h"
#include "yaFontWrapper.h"

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
		Fmod::Initialize();
		FontWrapper::Initialize();

		CollisionManager::Initialize();
		renderer::Initialize();
		SceneManager::Initialize();
	}

	void Application::Update()
	{
		RECT rect = {};
		GetWindowRect(mHwnd, &rect);
		mPosition.x = rect.left;
		mPosition.y = rect.top;
		mWidth = rect.right - rect.left;
		mHeight = rect.bottom - rect.top;

		Time::Update();
		Input::Update();
		

		SceneManager::Update();
		CollisionManager::Update();
	}

	void Application::FixedUpdate()
	{
		SceneManager::FixedUpdate();
	}

	void Application::Render()
	{
		graphicsDevice->Clear();
		graphicsDevice->AdjustViewport();

//#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)
//		FontWrapper::DrawFont(L"TEST", 10, 10, 100, FONT_RGBA(255, 0, 255, 255));
//		Time::Render();
		renderer::Render();
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
			mHeight = height;

			graphicsDevice = std::make_unique<GraphicsDevice_DX11>();
			
			//ya::device = graphicsDevice.get();
		}

		RECT rt = { 0, 0, (LONG)width , (LONG)height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(mHwnd, nullptr, mPosition.x, mPosition.y, rt.right - rt.left, rt.bottom - rt.top, 0);
		
		ShowWindow(mHwnd, true);
		UpdateWindow(mHwnd);

		RECT rect = {};
		GetWindowRect(mHwnd, &rect);
		mPosition.x = rect.left;
		mPosition.y = rect.top;
		mWidth  = rect.right - rect.left;
		mHeight = rect.bottom - rect.top;
	}

	void Application::SetPosition(int x, int y)
	{
		mPosition.x = (float)x;
		mPosition.y = (float)y;
	}

	void Application::GraphicDeviceResize()
	{
		if (graphicsDevice)
		{
			RECT winRect;
			GetClientRect(mHwnd, &winRect);
			D3D11_VIEWPORT viewport = {};
			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = 0.0f;
			viewport.Width = (FLOAT)(winRect.right - winRect.left);
			viewport.Height = (FLOAT)(winRect.bottom - winRect.top);
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;

			graphicsDevice->ReSizeGrphicDevice(viewport);

			mWidth = viewport.Width;
			mHeight = viewport.Height;
		}
	}

	void Application::Release()
	{
		SceneManager::Release();
		renderer::Release();
		Resources::Release();
		Fmod::Release();
		FontWrapper::Release();
	}
}
