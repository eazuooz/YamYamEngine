#pragma once
#include "..\\External\\Include\\Fmod\\fmod_studio.hpp"
#include "..\\External\\Include\\Fmod\\fmod.hpp"
#include "..\\External\\Include\\Fmod\\fmod_common.h"
#include "..\\External\\Include\\Fmod\\fmod_codec.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\Library\\Fmod\\Debug\\fmodL_vc.lib")
#pragma comment(lib, "..\\External\\Library\\Fmod\\Debug\\fmodstudioL_vc.lib")
#else
#pragma comment(lib, "..\\External\\Library\\Fmod\\Debug\\fmod_vc.lib")
#pragma comment(lib, "..\\External\\Library\\Fmod\\Release\\fmodstudio_vc.lib")
#endif

namespace ya
{
	class Fmod
	{
	public:
		static void Initialize();

	private:
		static FMOD::Studio::System* mSystem;
		static FMOD::System* mCoreSystem;
	};
}
