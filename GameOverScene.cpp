#include "GameOverScene.h"
#include <cstdio>

#include "MenuScene.h"


// -----------------------------------------------------------
// Handle absolute mouse movement
// -----------------------------------------------------------
void GameOverScene::MouseMoveAbsolute(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

// -----------------------------------------------------------
// Handle mouse input
// -----------------------------------------------------------
void GameOverScene::MouseDown(int button)
{
	if (button == 1) // 1 = left mouse button
	{
		mouseClicked = true;
	}
}

// -----------------------------------------------------------
// Initialize the game over scene
// -----------------------------------------------------------
void GameOverScene::Init()
{
}

// -----------------------------------------------------------
// Close down game over scene
// -----------------------------------------------------------
void GameOverScene::Shutdown()
{
}

// -----------------------------------------------------------
//	Tick function for the game over scene
// -----------------------------------------------------------
std::unique_ptr<Scene> GameOverScene::Tick(float deltaTime)
{
	if (mouseClicked)
		return std::make_unique<MenuScene>();

	return nullptr;
}

void GameOverScene::Draw(Tmpl8::Surface* screen)
{
	screen->Clear(0);
	screen->Print("Game Over", 350, 150, 0xFFFFFF);
	char buffer[32];
	sprintf(buffer, "Final Score: %d", score);
	screen->Print(buffer, 350, 200, 0xFFFFFF);
	screen->Print("Click to Restart", 350, 250, 0xFFFFFF);
}
