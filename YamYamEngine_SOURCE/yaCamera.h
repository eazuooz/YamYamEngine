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
		void RegisterCameraInRenderer();

		void TurnLayerMask(eLayer layer, bool enable = true);
		void EnableLayerMasks() { mLayerMasks.set(); }
		void DisableLayerMasks() { mLayerMasks.reset(); }

	private:
		void sortGameObjects();
		//void pushGameObjectToRenderingMode(eRenderingMode mode, GameObject* gameObject);
		void renderOpaque();
		void renderCutout();
		void renderTransparent();

	private:
		static Matrix mView;
		static Matrix mProjection;

		bool mbEnabled;
		eProjectionType mProjectionType;
		float mAspectRatio;

		float mNear;
		float mFar;
		float mScale;

		std::bitset<(UINT)eLayer::Max> mLayerMasks;
		std::vector<GameObject*> mOpaqueGameObjects;
		std::vector<GameObject*> mCutoutGameObjects;
		std::vector<GameObject*> mTransparentGameObjects;
	};
}

