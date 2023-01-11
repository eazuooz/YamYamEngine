#pragma once
#include "yaComponent.h"

namespace ya
{
	class Camera : public Component
	{
	public:
		enum eProjectionType
		{
			Perspective,
			Orthographic,
		};

		inline static Matrix GetViewMatrix() { return mView; }
		inline static Matrix GetProjectionMatrix() { return mProjection; }

		Camera();
		~Camera();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void CreateViewMatrix();
		void CreateProjectionMatrix(eProjectionType type);

	private:
		static Matrix mView;
		static Matrix mProjection;

		eProjectionType mProjectionType;
		float mAspectRatio;

		float mNear;
		float mFar;
		float mScale;
	};
}

