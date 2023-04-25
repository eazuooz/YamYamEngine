#include "guiGame.h"
#include "yaApplication.h"

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

	}

	void Game::LateUpdate()
	{
	}


}