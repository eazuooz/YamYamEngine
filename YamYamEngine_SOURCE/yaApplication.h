#pragma once
#include "yaGraphicsDevice_DX11.h"
#include <Windows.h>

namespace ya
{
	class Application 
	{
	public:
		Application();
		~Application();

		//  Runs the main engine loop
		void Run();

		//  This is where the critical initializations happen (before any rendering or anything else)
		void Initialize();

		//  This is where application-wide updates get executed once per frame. 
		//  RenderPath::Update is also called from here for the active component
		void Update();

		//  This is where application-wide updates get executed in a fixed timestep based manner. 
		//  RenderPath::FixedUpdate is also called from here for the active component
		void FixedUpdate();

		//  This is where application-wide rendering happens to offscreen buffers. 
		//  RenderPath::Render is also called from here for the active component
		void Render()  const;
		void Present() const;

		// You need to call this before calling Run() or Initialize() if you want to render
		void SetWindow(const HWND hwnd, const UINT width, const UINT height);
		void SetPosition(int x, int y);
		void GraphicDeviceResize();
		void Release();

		[[nodiscard]] UINT GetWidth() const { return mWidth; }
		[[nodiscard]] UINT GetHeight() const { return mHeight; }
		[[nodiscard]] Vector2 getPosition() const { return mPosition; }
		[[nodiscard]] HWND GetHwnd() const { return mHwnd; }

	private:
		bool initialized = false;
		std::unique_ptr<ya::graphics::GraphicsDevice_DX11> graphicsDevice;

		HWND mHwnd;
		UINT mWidth;
		UINT mHeight;
		Vector2 mPosition;
	};
}
