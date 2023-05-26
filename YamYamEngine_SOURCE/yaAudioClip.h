#pragma once
#include "yaResource.h"
#include "yaFmod.h"



//#include <Fmod/fmod_studio.hpp>
//#include <Fmod/fmod.hpp>
//#include <Fmod/fmod_common.h>
//#include <Fmod/fmod_codec.h>

//#include "..\External\Include\\DirectXTex\DirectXTex.h"

namespace ya
{
	using namespace math;
	class AudioClip : public Resource
	{
	public:
		AudioClip();
		~AudioClip();

		virtual HRESULT Load(const std::wstring& path) override;

	private:
		void playSound(bool loop);
		void set3DAttributes(const Vector3 pos, const Vector3 vel);


	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;
	};
}
