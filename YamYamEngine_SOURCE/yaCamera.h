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

		inline static Matrix GetGpuViewMatrix() { return View; }
		inline static Matrix GetGpuProjectionMatrix() { return Projection; }
		inline static void SetGpuViewMatrix(Matrix matrix) { View = matrix; }
		inline static void SetGpuProjectionMatrix(Matrix matrix) { Projection = matrix; }

		Camera();
		~Camera();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

		void CreateViewMatrix();
		void CreateProjectionMatrix(eProjectionType type);
		void RegisterCameraInRenderer();

		void EnableDepthStencilState();
		void DisableDepthStencilState();

		void ZSortTransparencyGameObjects();
		void AlphaSortGameObjects();

		void DivideAlphaBlendGameObjects(const std::vector<GameObject*>& gameObjects);
		void RenderOpaque();
		void RenderCutout();
		void RenderTransparent();
		void RenderPostProcess();

		void TurnLayerMask(eLayerType layer, bool enable = true);
		void EnableLayerMasks() { mLayerMasks.set(); }
		void DisableLayerMasks() { mLayerMasks.reset(); }

		float GetOrthographicScale() { return mScale; }
		void SetOrthographicScale(float scale) { mScale = scale; }

	private:
		static Matrix View;
		static Matrix Projection;

		bool mbMain;
		eProjectionType mProjectionType;
		float mAspectRatio;

		float mNear;
		float mFar;
		float mScale;

		Matrix mView;
		Matrix mProjection;

		std::bitset<(UINT)eLayerType::Max> mLayerMasks;
		std::vector<GameObject*> mOpaqueGameObjects;
		std::vector<GameObject*> mCutoutGameObjects;
		std::vector<GameObject*> mTransparentGameObjects;
		std::vector<GameObject*> mPostProcessGameObjects;
	};
}

