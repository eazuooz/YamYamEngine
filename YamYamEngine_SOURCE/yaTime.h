#pragma once
#include "CommonInclude.h"
#include "YamYamEngine.h"

namespace ya
{
	class Time
	{
	public:
		static void Initialize();
		static void Update();
		static void Render();

		static __forceinline float DeltaTime() { return mDeltaTime; }

	private:
		static LARGE_INTEGER	mCpuFrequency;
		static LARGE_INTEGER   mPrevFrequency;
		static LARGE_INTEGER	mCurFrequency;

		static float			mDeltaTime;
		static float			mOneSecond;
	};
}