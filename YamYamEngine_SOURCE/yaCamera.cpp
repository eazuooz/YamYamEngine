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

extern ya::Application application;

namespace ya
{
	Matrix Camera::mView = Matrix::Identity;
	Matrix Camera::mProjection = Matrix::Identity;

	Camera::Camera()
		: Component(eComponentType::Camera)
		, mProjectionType(eProjectionType::Perspective)
		, mNear(1.0f)
		, mFar(1000.0f)
	{
	}
	Camera::~Camera()
	{
	}
	void Camera::Initialize()
	{
		EnableLayerMasks();
	}
	void Camera::Update()
	{
		Vector3 pos = GetOwner()->GetComponent<Transform>()->GetPosition();
		if (Input::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
		{
			pos.z += 5.0f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
		{
			pos.x -= 5.0f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
		{
			pos.z -= 5.0f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
		{
			pos.x += 5.0f * Time::DeltaTime();
		}
		GetOwner()->GetComponent<Transform>()->SetPosition(pos);
	}
	void Camera::FixedUpdate()
	{
		CreateViewMatrix();
		CreateProjectionMatrix(mProjectionType);
		RegisterCameraInRenderer();
	}

	void Camera::Render()
	{
		sortGameObjects();

		renderOpaque();
		renderCutout();
		renderTransparent();
	}

	void Camera::CreateViewMatrix()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		// View Translate Matrix
		Camera::mView = Matrix::Identity;
		Camera::mView *= Matrix::CreateTranslation(-pos);

		// View rotation Matrix
		Vector3 up = tr->Up();
		Vector3 foward = tr->Foward();
		Vector3 right = tr->Right();
		
		Matrix viewRotate;
		viewRotate._11 = right.x; viewRotate._12 = up.x; viewRotate._13 = foward.x;
		viewRotate._21 = right.y; viewRotate._22 = up.y; viewRotate._23 = foward.y;
		viewRotate._31 = right.z; viewRotate._32 = up.z; viewRotate._33 = foward.z;

		Camera::mView *= viewRotate;
	}
	void Camera::CreateProjectionMatrix(eProjectionType type)
	{
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		float width = winRect.right - winRect.left;
		float height = winRect.bottom - winRect.top;
		float aspectRatio = width / height;
		
		if (type == eProjectionType::Perspective)
			Camera::mProjection = Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, aspectRatio, mNear, mFar);
		else if (type == eProjectionType::Orthographic)
			Camera::mProjection = Matrix::CreateOrthographicLH(width, height, mNear, mFar);
	}

	void Camera::RegisterCameraInRenderer()
	{
		renderer::cameras.push_back(this);
	}

	void Camera::TurnLayerMask(eLayer layer, bool enable)
	{
		mLayerMasks.set((UINT)layer, enable);
	}

	void Camera::sortGameObjects()
	{
		mOpaqueGameObjects.clear();
		mCutoutGameObjects.clear();
		mTransparentGameObjects.clear();

		Scene* scene = SceneManager::GetActiveScene();
		for (size_t i = 0; i < (UINT)eLayer::Max; i++)
		{
			// layer check
			if (mLayerMasks[i] == true)
			{
				// get gameobjects in layer
				Layer* layer = scene->GetLayer((eLayer)i);
				const std::vector<GameObject*>& gameObjects = layer->GetGameObjects();
				
				if (gameObjects.size() == 0)
					continue;

				pushGameObjectToRenderingModes(gameObjects);
			}
		} 
	}

	void Camera::pushGameObjectToRenderingModes(const std::vector<GameObject*>& gameObjects)
	{
		for (GameObject* obj : gameObjects)
		{
			MeshRenderer* meshRenderer = obj->GetComponent<MeshRenderer>();
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
			case ya::graphics::eRenderingMode::End:
				break;
			default:
				break;
			}
		}
	}



	void Camera::renderOpaque()
	{
		for (GameObject* obj : mOpaqueGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Camera::renderCutout()
	{
		for (GameObject* obj : mCutoutGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}

	void Camera::renderTransparent()
	{
		for (GameObject* obj : mTransparentGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}
}
