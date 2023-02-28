#include "yaCameraScript.h"
#include "yaGameObject.h"
#include "yaTransform.h"
#include "yaInput.h"
#include "yaTime.h"
namespace ya
{
	CameraScript::CameraScript()
	{
	}
	CameraScript::~CameraScript()
	{
	}
	void CameraScript::Initialize()
	{
	}
	void CameraScript::Update()
	{
		Vector3 pos = GetOwner()->GetComponent<Transform>()->GetPosition();
		Transform* tr = GetOwner()->GetComponent<Transform>();

		if (Input::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
		{
			pos += 50.0f * tr->Foward() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
		{
			pos += 50.0f * -tr->Foward() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
		{
			pos += 50.0f * -tr->Right() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
		{
			pos += 50.0f * tr->Right() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::Q) == eKeyState::PRESSED)
		{
			pos += 50.0f * -tr->Up() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::E) == eKeyState::PRESSED)
		{
			pos += 50.0f * tr->Up() * Time::DeltaTime();
		}

		GetOwner()->GetComponent<Transform>()->SetPosition(pos);
	}
}
