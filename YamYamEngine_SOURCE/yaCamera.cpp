#include "yaCamera.h"
#include "yaConstantBuffer.h"
#include "yaRenderer.h"
#include "yaApplication.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaInput.h"
#include "yaTime.h"
#include "yaScene.h"
#include "yaSceneManager.h"
#include "yaShader.h"
#include "yaMesh.h"
#include "yaMaterial.h"
#include "yaMeshRenderer.h"
#include "yaSceneManager.h"
extern ya::Application application;

namespace ya
{
	bool CompareZSort(GameObject* a, GameObject* b)
	{
		if (a->GetComponent<Transform>()->GetPosition().z
			< b->GetComponent<Transform>()->GetPosition().z)
			return false;

		return true;
	}

	Matrix Camera::View = Matrix::Identity;
	Matrix Camera::Projection = Matrix::Identity;

	Camera::Camera()
		: Component(eComponentType::Camera)
		, mProjectionType(eProjectionType::Perspective)
		, mNear(1.0f)
		, mFar(1000.0f)
		, mScale(1.0f)
		, mView(Matrix::Identity)
		, mProjection(Matrix::Identity)
	{
		EnableLayerMasks();
	}
	Camera::~Camera()
	{
	}
	void Camera::Initialize()
	{
		
	}
	void Camera::Update()
	{

	}
	void Camera::LateUpdate()
	{
		CreateViewMatrix();
		CreateProjectionMatrix(mProjectionType);
		RegisterCameraInRenderer();
	}

	void Camera::Render()
	{
		View = mView;
		Projection = mProjection;

		AlphaSortGameObjects();
		ZSortTransparencyGameObjects();
		RenderOpaque();

		//DisableDepthStencilState();
		RenderCutout();
		RenderTransparent();
		//EnableDepthStencilState();

		RenderPostProcess();
	}

	void Camera::CreateViewMatrix()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		// View Translate Matrix
		mView = Matrix::Identity;
		mView *= Matrix::CreateTranslation(-pos);

		// View rotation Matrix
		Vector3 up = tr->Up();
		Vector3 foward = tr->Foward();
		Vector3 right = tr->Right();
		
		Matrix viewRotate;
		viewRotate._11 = right.x; viewRotate._12 = up.x; viewRotate._13 = foward.x;
		viewRotate._21 = right.y; viewRotate._22 = up.y; viewRotate._23 = foward.y;
		viewRotate._31 = right.z; viewRotate._32 = up.z; viewRotate._33 = foward.z;

		mView *= viewRotate;
	}
	void Camera::CreateProjectionMatrix(eProjectionType type)
	{
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		float width = (winRect.right - winRect.left) * mScale;
		float height = (winRect.bottom - winRect.top) * mScale;
		float aspectRatio = width / height;

		
		if (type == eProjectionType::Perspective)
			mProjection = Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, aspectRatio, mNear, mFar);
		else if (type == eProjectionType::Orthographic)
			mProjection = Matrix::CreateOrthographicLH(width, height, mNear, mFar);
	}

	void Camera::RegisterCameraInRenderer()
	{
		eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		renderer::cameras[(UINT)type].push_back(this);
	}

	void Camera::TurnLayerMask(eLayerType layer, bool enable)
	{
		mLayerMasks.set((UINT)layer, enable);
	}

	void Camera::EnableDepthStencilState()
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState
			= renderer::depthStencilStates[(UINT)eDSType::Less];
		GetDevice()->BindDepthStencilState(dsState.Get());
	}

	void Camera::DisableDepthStencilState()
	{
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState
			= renderer::depthStencilStates[(UINT)eDSType::None];
		GetDevice()->BindDepthStencilState(dsState.Get());
	}

	void Camera::ZSortTransparencyGameObjects()
	{
		std::sort(mCutoutGameObjects.begin()
			, mCutoutGameObjects.end()
			, CompareZSort);
		std::sort(mTransparentGameObjects.begin()
			, mTransparentGameObjects.end()
			, CompareZSort);
	}

	void Camera::AlphaSortGameObjects()
	{
		mOpaqueGameObjects.clear();
		mCutoutGameObjects.clear();
		mTransparentGameObjects.clear();
		mPostProcessGameObjects.clear();

		Scene* scene = SceneManager::GetActiveScene();
		for (size_t i = 0; i < (UINT)eLayerType::Max; i++)
		{
			// layer check
			if (mLayerMasks[i] == true)
			{
				// get gameobjects in layer
				Layer* layer = scene->GetLayer((eLayerType)i);
				const std::vector<GameObject*>& gameObjects = layer->GetGameObjects();
				
				if (gameObjects.size() == 0)
					continue;

				DivideAlphaBlendGameObjects(gameObjects);
			}
		} 
	}

	void Camera::DivideAlphaBlendGameObjects(const std::vector<GameObject*>& gameObjects)
	{
		for (GameObject* obj : gameObjects)
		{
			GameObject::eState state = obj->GetActive();
			if (state == GameObject::eState::Dead
				|| state == GameObject::eState::Paused)
				continue;

			BaseRenderer* meshRenderer = obj->GetComponent<BaseRenderer>();
			if (meshRenderer == nullptr)
				continue;

			Material* material = meshRenderer->GetMaterial().get();
			eRenderingMode mode = material->GetRenderingMode();

			switch (mode)
			{
			case ya::graphics::eRenderingMode::Opaque:
				mOpaqueGameObjects.push_back(obj);
				break;
			case ya::graphics::eRenderingMode::CutOut:
				mCutoutGameObjects.push_back(obj);
				break;
			case ya::graphics::eRenderingMode::Transparent:
				mTransparentGameObjects.push_back(obj);
				break;
			case ya::graphics::eRenderingMode::PostProcess:
				mPostProcessGameObjects.push_back(obj);
				break;
			case ya::graphics::eRenderingMode::End:
				break;
			default:
				break;
			}
		}
	}



	void Camera::RenderOpaque()
	{
		for (GameObject* obj : mOpaqueGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Camera::RenderCutout()
	{
		for (GameObject* obj : mCutoutGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Camera::RenderTransparent()
	{
		for (GameObject* obj : mTransparentGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Camera::RenderPostProcess()
	{
		for (GameObject* obj : mPostProcessGameObjects)
		{
			renderer::CopyRenderTarget();
			obj->Render();
		}
	}
}
