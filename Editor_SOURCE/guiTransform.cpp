#include "guiTransform.h"
#include "yaTransform.h"

namespace gui
{
	Transform::Transform()
		: Component(ya::enums::eComponentType::Transform)
	{
		SetName("Transform");
	}

	Transform::~Transform()
	{

	}

	void Transform::FixedUpdate()
	{
		Component::FixedUpdate();
		if (GetTarget() == nullptr)
			return;

		ya::Transform* tr = GetTarget()->GetComponent<ya::Transform>();

		mPosition = tr->GetPosition();
		mRotation = tr->GetRotation();
		mScale = tr->GetScale();
	}

	void Transform::Update()
	{
		Component::Update();
		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::InputFloat3("##Position", (float*)&mPosition);

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", (float*)&mRotation);

		ImGui::Text("Scale   "); ImGui::SameLine();
		ImGui::InputFloat3("##Scale", (float*)&mScale);

		if (GetTarget())
		{
			ya::Transform* tr = GetTarget()->GetComponent<ya::Transform>();
			tr->SetPosition(mPosition);
			tr->SetRotation(mRotation);
			tr->SetScale(mScale);
		}
	}

	void Transform::LateUpdate()
	{
		Component::LateUpdate();
	}
}
