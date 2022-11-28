#include "yaApplication.h"
#include "yaGraphicsDevice_DX11.h"

using namespace ya::graphics;

namespace ya
{
	void Application::Run()
	{
		graphicsDevice->Draw();
	}

	void Application::Initialize()
	{
		
	}

	void Application::Update(float dt)
	{

	}

	void Application::FixedUpdate()
	{

	}

	void Application::Render()
	{

	}

	void Application::SetWindow(HWND hwnd, UINT width, UINT height)
	{
		if (graphicsDevice == nullptr)
		{
			mHwnd = hwnd;
			mWidth = width;
			mHegith = height;

			ValidationMode validationMode = ValidationMode::Disabled;
			graphicsDevice = std::make_unique<GraphicsDevice_DX11>(validationMode);
		}

		RECT rt = { 0, 0, (LONG)width , (LONG)height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(mHwnd, true);
		UpdateWindow(mHwnd);
	}
}
