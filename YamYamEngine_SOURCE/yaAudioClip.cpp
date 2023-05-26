#include "yaAudioClip.h"
#include "yaTransform.h"


namespace ya
{
	AudioClip::AudioClip()
		: Resource(eResourceType::AudioClip)
		, mSound(nullptr)
		, mChannel(nullptr)
	{

	}

	AudioClip::~AudioClip()
	{
	}

	HRESULT AudioClip::Load(const std::wstring& path)
	{
		std::string cPath(path.begin(), path.end());
		if (!Fmod::CreateSound(cPath, mSound))
			return S_FALSE;

		mSound->set3DMinMaxDistance(1.0f, 1000.0f);
		return S_OK;
	}

	void AudioClip::playSound(bool loop)
	{
		if (loop)
			mSound->setMode(FMOD_LOOP_NORMAL);
		else
			mSound->setMode(FMOD_LOOP_OFF);

		Fmod::SoundPlay(mSound, &mChannel);
	}

	void AudioClip::set3DAttributes(const Vector3 pos, const Vector3 vel)
	{
		FMOD_VECTOR fmodPos(pos.x, pos.y, pos.z);
		FMOD_VECTOR fmodVel(vel.x, vel.y, vel.z);

		mChannel->set3DAttributes(&fmodPos, &fmodVel);
	}

	
}
