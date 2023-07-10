#include "yaPlayer.h"
#include "yaInput.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaTime.h"

namespace ya
{
	Player::Player()
	{
	}

	Player::~Player()
	{
	}

	void Player::Initialize()
	{
	}

	void Player::Update()
	{
		Vector3 pos = GetOwner()->GetComponent<Transform>()->GetPosition();
		Transform* tr = GetOwner()->GetComponent<Transform>();
		if (Input::GetKeyState(eKeyCode::UP) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr->Up() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::DOWN) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr->Up() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::LEFT) == eKeyState::PRESSED)
		{
			pos += 100.0f * -tr->Right() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::RIGHT) == eKeyState::PRESSED)
		{
			pos += 100.0f * tr->Right() * Time::DeltaTime();
		}
		GetOwner()->GetComponent<Transform>()->SetPosition(pos);
	}
}
