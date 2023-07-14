#pragma once
#include "yaGraphicsDevice_DX11.h"
#include <Windows.h>

namespace ya
{
	class Application
	{
	public:
		virtual ~Application();

		//  Runs the main engine loop
		void Run();

		//  This is where the critical initializations happen (before any rendering or anything else)
		virtual void Initialize();

		//  This is where application-wide updates get executed once per frame. 
		//  RenderPath::Update is also called from here for the active component
		virtual void Update();

		//  This is where application-wide updates get executed in a fixed timestep based manner. 
		//  RenderPath::FixedUpdate is also called from here for the active component
		virtual void FixedUpdate();

		//  This is where application-wide rendering happens to offscreen buffers. 
		//  RenderPath::Render is also called from here for the active component
		virtual void Render();
		virtual void Present();

		// You need to call this before calling Run() or Initialize() if you want to render
		void SetWindow(HWND hwnd, UINT width, UINT height);
		void SetPosition(int x, int y);
		void GraphicDeviceResize();
		void Release();

		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHegith; }
		Vector2 GetPosition() { return mPosition; }
		Vector2 GetSize() { return mSize; }
		void SetSize(Vector2 size) { mSize = size; }
		HWND GetHwnd() { return mHwnd; }

	private:
		bool initialized = false;
		std::unique_ptr<ya::graphics::GraphicsDevice_DX11> graphicsDevice;

		HWND mHwnd;
		UINT mWidth;
		UINT mHegith;
		Vector2 mPosition;
		Vector2 mSize;
	};
}
