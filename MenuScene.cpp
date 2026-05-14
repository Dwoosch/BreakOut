#include "MenuScene.h"

#include "GameScene.h"
#include "GameState.h"
#include "miniaudio.h"
#include "template.h"

// -----------------------------------------------------------
// Handle absolute mouse movement
// -----------------------------------------------------------
void MenuScene::MouseMoveAbsolute(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

// -----------------------------------------------------------
// Handle mouse input
// -----------------------------------------------------------
void MenuScene::MouseDown(int button)
{
	if (button == 1) // 1 = left mouse button
	{
		mouseClicked = true;
		ma_engine_play_sound(&engine, "assets/start.mp3", NULL);
	}
}

// -----------------------------------------------------------
// Initialize the menu scene
// -----------------------------------------------------------
void MenuScene::Init()
{
	for (int i = 0; i < 6; i++)
	{
		menuBalls[i].x = IRand(800); // random x position within the screen width
		menuBalls[i].y = IRand(600); // random y position within the screen height
		menuBalls[i].dx = (IRand(2) == 0 ? 1 : -1) * (IRand(4) + 1); // random horizontal velocity between -5 and 5 (excluding 0)
		menuBalls[i].dy = (IRand(2) == 0 ? 1 : -1) * (IRand(4) + 1); // random vertical velocity between -5 and 5 (excluding 0)
	}
}

// -----------------------------------------------------------
// Close down menu scene
// -----------------------------------------------------------
void MenuScene::Shutdown()
{
}

// -----------------------------------------------------------
//	Tick function for the menu scene
// -----------------------------------------------------------
std::unique_ptr<Scene> MenuScene::Tick(float deltaTime)
{
	if (mouseClicked)
		return std::make_unique<GameScene>();

	return nullptr;
}

void MenuScene::Draw(Tmpl8::Surface* screen)
{
	// clear the screen with a solid color
	screen->Clear(0);
	// draw the menu text
	screen->Print("Welcome to Breakout!", 350, 150, 0xFFFFFF);
	screen->Print("Click to Start", 350, 200, 0xFFFFFF);

    for (int i = 0; i < 6; i++)
	{
		menuBalls[i].Move(true);
		menuBalls[i].Draw(screen);
	}
}
