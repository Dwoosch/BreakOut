#include "game.h"
#include "MenuScene.h"

namespace Tmpl8 {

	// Credits to Jeremiah van Oosten for helping me with the scene management! https://discord.gg/gsxxaxc

	// -----------------------------------------------------------
	// Handle mouse input
	// -----------------------------------------------------------
	void Game::MouseDown(int button)
	{
		currentScene->MouseDown(button);
	}

	// -----------------------------------------------------------
	// Handle absolute mouse movement
	// -----------------------------------------------------------
	void Game::MouseMoveAbsolute(int x, int y)
	{
		currentScene->MouseMoveAbsolute(x, y);
	}

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		Audio::Init();
		Audio::SetVolume(0.5f); // set global volume to 50%
		currentScene = std::make_unique<MenuScene>();
		currentScene->Init();
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
		currentScene->Shutdown();
	}
		
	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		auto nextScene = currentScene->Tick(deltaTime);
		currentScene->Draw(screen);

		if (nextScene)
		{
			currentScene->Shutdown();
			currentScene = std::move(nextScene);
			currentScene->Init();
		}
	}
}