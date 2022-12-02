#include "yaApplication.h"
#include "yaGraphicsDevice_DX11.h"

#include "yaInput.h"
#include "yaTime.h"
#include "yaRenderer.h"

using namespace ya::graphics;

namespace ya
{
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
	}

	void Application::Update()
	{
		Time::Update();
		Input::Update();
	}

	void Application::FixedUpdate()
	{

	}

	void Application::Render()
	{
		Time::Render();
		graphicsDevice->Draw();
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
			ya::graphics::GetDevice() = graphicsDevice.get();
			//ya::device = graphicsDevice.get();
		}

		RECT rt = { 0, 0, (LONG)width , (LONG)height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(mHwnd, true);
		UpdateWindow(mHwnd);
	}
}
