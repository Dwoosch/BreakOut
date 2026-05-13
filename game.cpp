#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"	
#include "game.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "GameOverScene.h"

ma_engine engine;

namespace Tmpl8 {
	MenuScene menuScene;
	GameScene gameScene;
	GameOverScene gameOverScene;
	bool engineInitialized = false;

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
		if (ma_engine_init(NULL, &engine) == MA_SUCCESS)
			engineInitialized = true;
		ma_engine_set_volume(&engine, 0.3f); // 0.0 to 1.0
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
		if (engineInitialized)
		{
			ma_engine_uninit(&engine);
			engineInitialized = false;
		}
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