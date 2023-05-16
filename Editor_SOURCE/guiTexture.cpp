#include "guiTexture.h"
#include "yaTexture.h"

namespace gui
{
	Texture::Texture()
	{

	}

	Texture::~Texture()
	{

	}

	void Texture::Update()
	{
		ya::graphics::Texture* pTargetTex 
			= (ya::graphics::Texture*)(GetTarget());

		if (pTargetTex == nullptr)
			return;

		ImGui::Image(pTargetTex->GetSRV().Get(), ImVec2(150.f, 150.f));

		std::string strKey(pTargetTex->GetKey().begin(), pTargetTex->GetKey().end());
		std::string strRelativePath(pTargetTex->GetPath().begin(), pTargetTex->GetPath().end());

		ImGui::Text("Key   ");
		ImGui::SameLine();
		ImGui::InputText("##TexKey", (char*)strKey.data(), strKey.length(), ImGuiInputTextFlags_ReadOnly);

		ImGui::Text("Path  ");
		ImGui::SameLine();
		ImGui::InputText("##TexPath", (char*)strRelativePath.data(), strRelativePath.length(), ImGuiInputTextFlags_ReadOnly);

		char szNum[50] = {};
		_itoa_s(pTargetTex->GetWidth(), szNum, 10);


		ImGui::Text("Width ");
		ImGui::SameLine();
		ImGui::InputText("##TexWidth", szNum, 50, ImGuiInputTextFlags_ReadOnly);

		_itoa_s(pTargetTex->GetHeight(), szNum, 10);
		ImGui::Text("Height");
		ImGui::SameLine();
		ImGui::InputText("##TexHeight", szNum, 50, ImGuiInputTextFlags_ReadOnly);
	}

	void Texture::LateUpdate()
	{

	}
}
