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

	void Transform::Update()
	{
		ya::Transform* tr = GetTarget()->GetComponent<ya::Transform>();

		mPosition = tr->GetPosition();
		mRotation = tr->GetRotation();
		mScale = tr->GetScale();

		Component::Update();
	}

	void Transform::LateUpdate()
	{
		Component::LateUpdate();

		ImGui::Text("Position"); ImGui::SameLine(); 
		ImGui::InputFloat3("##Position", (float*)&mPosition);

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", (float*)&mRotation);

		ImGui::Text("Scale   "); ImGui::SameLine(); 
		ImGui::InputFloat3("##Scale", (float*)&mScale);

		//ImGui::Text("Ignore Parent Scale"); 
		//ImGui::SameLine(); ImGui::Checkbox("##IgnorParentScale", &m_bIgnorScale);

		if (GetTarget())
		{
			ya::Transform* tr = GetTarget()->GetComponent<ya::Transform>();

			tr->SetPosition(mPosition);
			tr->SetRotation(mRotation);
			tr->SetScale(mScale);
		}
	}
}
