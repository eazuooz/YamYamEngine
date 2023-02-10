#pragma once
#include "yaScript.h"
#include "yaCamera.h"

namespace ya
{
	using namespace enums;
	class GridScript : public Script
	{
	public:
		GridScript();
		~GridScript();

		virtual void Initialize() override;
		virtual void Update() override;

		void SetCamera(Camera* camera) { mCamera = camera; }

	private:
		Camera* mCamera;
	};
}
