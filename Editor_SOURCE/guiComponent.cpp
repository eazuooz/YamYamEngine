#include "guiComponent.h"


namespace gui
{

	Component::Component(ya::enums::eComponentType type)
		: mType(type)
		, mTarget(nullptr)
	{

	}

	Component::~Component()
	{

	}



	void Component::FixedUpdate()
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
		ImGui::Button(ya::charComponentType[(UINT)mType]);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

	void Component::Update()
	{

	}

	void Component::LateUpdate()
	{

	}
}