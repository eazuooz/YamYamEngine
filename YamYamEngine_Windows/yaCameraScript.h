#pragma once
#include "yaScript.h"


namespace ya
{
	using namespace enums;
	class CameraScript : public Script
	{
	public:
		CameraScript();
		~CameraScript();

		virtual void Initialize() override;
		virtual void Update() override;

	private:
	};
}