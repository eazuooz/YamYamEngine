#pragma once
#include "yaScript.h"
#include "yaCamera.h"

namespace ya
{
	using namespace enums;
	class Player : public Script
	{
	public:
		Player();
		~Player();

		virtual void Initialize() override;
		virtual void Update() override;


	private:
		
	};
}

