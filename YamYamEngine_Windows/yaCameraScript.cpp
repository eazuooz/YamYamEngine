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
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = GetOwner()->GetComponent<Transform>()->getPosition();
		

		if (Input::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
		{
			pos += 20.0f * tr->Foward() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
		{
			pos += 20.0f * -tr->Foward() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
		{
			pos += 20.0f * -tr->Right() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
		{
			pos += 20.0f * tr->Right() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::Q) == eKeyState::PRESSED)
		{
			pos += 20.0f * -tr->Up() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::E) == eKeyState::PRESSED)
		{
			pos += 20.0f * tr->Up() * Time::DeltaTime();
		}

		GetOwner()->GetComponent<Transform>()->SetPosition(pos);

		Vector3 rot = GetOwner()->GetComponent<Transform>()->GetRotation();
		
		if (Input::GetKeyState(eKeyCode::LEFT) == eKeyState::PRESSED)
		{
			rot.y += 100.0f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::RIGHT) == eKeyState::PRESSED)
		{
			rot.y -= 100.0f *  Time::DeltaTime();
		}

		GetOwner()->GetComponent<Transform>()->SetRotation(rot);
	}
}
