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
		std::shared_ptr<ya::graphics::Texture> renderTarget
			= ya::graphics::GetDevice()->GetRenderTargetTexture();

		std::shared_ptr<ya::graphics::Texture> gameTarget 
			= std::make_shared<Texture>();
		gameTarget->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

		ID3D11ShaderResourceView* srv = nullptr;
		ya::graphics::GetDevice()->BindShaderResource(eShaderStage::PS, 61, &srv);

		ya::graphics::GetDevice()->CopyResource(gameTarget->GetTexture().Get()
			, renderTarget->GetTexture().Get());

		gameTarget->BindShaderResource(eShaderStage::PS, 61);

		ImGuiIO& io = ImGui::GetIO();
		ImVec2 panelSize = ImGui::GetWindowSize();
		ImVec4 tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Set the tint color
		ImVec2 scene_size = ImVec2(io.DisplaySize.x /** 0.6f - 16.0f*/, io.DisplaySize.y /** 0.7f - 41.0f*/);
		
		ImGui::Image((void*)gameTarget->GetSRV().Get(), panelSize
			, ImVec2(0, 0), ImVec2(1, 1), tintCol, ImVec4(0,0,0,0));
	}

	void Game::LateUpdate()
	{

	}
}