#pragma once
#include "YamYamEngine.h"

#include "..\\External\\Include\\FW1FontWrapper\\FW1FontWrapper.h"
#include "..\\External\\Include\\FW1FontWrapper\\FW1CompileSettings.h"
//#include "..\\External\\Include\\Fmod\\common_platform.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\Library\\FW1FontWrapper\\Debug\\FW1FontWrapperL.lib")
#else
#pragma comment(lib, "..\\External\\Library\\FW1FontWrapper\\Debug\\FW1FontWrapper.lib")
#endif

//#include <SpriteFont.h>
//#include <SpriteBatch.h>

namespace ya
{
	class FontWrapper
	{
	public:
		static bool Initialize();
		static void DrawFont(const wchar_t* str, float x, float y, float size, UINT rgb);
		static void Release();


	private:
		static IFW1Factory* mFW1Factory;
		static IFW1FontWrapper* mFontWrapper;
	};
}
