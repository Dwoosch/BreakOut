#include "game.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "GameOverScene.h"

namespace Tmpl8 {
	MenuScene menuScene;
	GameScene gameScene;
	GameOverScene gameOverScene;

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
		menuScene.SetTargetScene(&gameScene);
		gameScene.SetTargetScene(&gameOverScene);
		gameOverScene.SetTargetScene(&menuScene);
		currentScene = &menuScene;
		currentScene->screen = screen;
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
		currentScene->Tick(deltaTime);

		if (currentScene->nextScene != nullptr)
		{
			currentScene->Shutdown();
			currentScene = currentScene->nextScene;
			currentScene->nextScene = nullptr;
			currentScene->screen = screen;
			currentScene->Init();
		}
	}
}