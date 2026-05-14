#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"	
#include "game.h"
#include "MenuScene.h"


ma_engine engine;

namespace Tmpl8 {
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
		currentScene = std::make_unique<MenuScene>();
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