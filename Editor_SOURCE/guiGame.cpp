#include "guiGame.h"
#include "yaApplication.h"
#include "yaTexture.h"
#include "yaResources.h"
#include "yaGraphicsDevice_DX11.h"
#include "yaRenderer.h"

extern ya::Application application;
namespace gui
{
	Game::Game()
	{
		SetName("Game");
		UINT width = application.GetWidth();
		UINT height = application.GetHeight();

		Vector2 size = application.GetSize();

		SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));
	}

	Game::~Game()
	{

	}

	void Game::Update()
	{
		std::shared_ptr<ya::graphics::Texture> tex
			= ya::graphics::GetDevice()->GetRenderTargetTexture();

		std::shared_ptr<ya::graphics::Texture> gameTex = std::make_shared<Texture>();
		gameTex->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

		ID3D11ShaderResourceView* srv = nullptr;
		ya::graphics::GetDevice()->BindShaderResource(eShaderStage::PS, 61, &srv);

		ya::graphics::GetDevice()->CopyResource(gameTex->GetTexture().Get()
			, tex->GetTexture().Get());

		gameTex->BindShaderResource(eShaderStage::PS, 61);

		//ImGui::GetStyle().Alpha = 0.5f;

		//ImGuiStyle& style = ImGui::GetStyle();
		//style.Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

		ImGuiIO& io = ImGui::GetIO();
		ImVec2 panelSize = ImGui::GetWindowSize();
		ImVec4 tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Set the tint color
		ImVec2 scene_size = ImVec2(io.DisplaySize.x /** 0.6f - 16.0f*/, io.DisplaySize.y /** 0.7f - 41.0f*/);
		ImGui::Image((void*)gameTex->GetSRV().Get(), panelSize
			, ImVec2(0, 0), ImVec2(1, 1), tintCol, ImVec4(0,0,0,0));


		//ImGui::GetStyle().Alpha = 1.0f;
		//ImGui::Image((void*)ya::renderer::postProcessTexture->GetSRV().Get(), scene_size, ImVec2(0, 1), ImVec2(1, 0));
	}

	void Game::LateUpdate()
	{

	}
}