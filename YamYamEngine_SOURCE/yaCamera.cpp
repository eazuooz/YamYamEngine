#include "yaCamera.h"
#include "yaConstantBuffer.h"
#include "yaRenderer.h"
#include "yaApplication.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaInput.h"
#include "yaTime.h"

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


		//ConstantBuffer* cb = renderer::constantBuffers[(UINT)graphics::eCBType::Transform];
		//cb->Bind(&trCB);
		//cb->SetPipline(graphics::eShaderStage::VS);
	}

	void Camera::Render()
	{
	}
	void Camera::CreateViewMatrix()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		// View Translate Matrix
		Camera::mView = Matrix::Identity;
		Camera::mView *= Matrix::CreateTranslation(-pos);

		// View rotation Matrix
		Vector3 up = tr->GetUp();
		Vector3 foward = tr->GetFoward();
		Vector3 right = tr->GetRight();
		
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
}
