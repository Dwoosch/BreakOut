#include "game.h"
#include "template.h"
#include "surface.h"
#include <cstdio> //printf
#include "ball.h"
#include "paddle.h"
#include "brick.h"

namespace Tmpl8
{
	const int BRICK_ROWS = 8;
	const int BRICK_COLUMNS = 8;
	const int GAP = 2; // pixels between bricks
	const int GRID_WIDTH = BRICK_ROWS * (Brick::BRICK_WIDTH + GAP) - GAP; // subtract trailing gap
	const int OFFSET_X = (ScreenWidth - GRID_WIDTH) / 2;

	Ball ball;
	Paddle paddle;
	Brick bricks[BRICK_ROWS][BRICK_COLUMNS];


	// -----------------------------------------------------------
	// Handle mouse movement
	// -----------------------------------------------------------
	void Game::MouseMoveAbsolute(int x, int y)
	{
		mouseX = x;
		mouseY = y;
	}

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		for (int i = 0; i < BRICK_ROWS; i++)
		{
			for (int j = 0; j < BRICK_COLUMNS; j++)
			{
				bricks[i][j].powerup = (Brick::PowerupType)IRand(3);
			}
		}
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		screen->Clear(0); // clear the screen to black
		paddle.Move(mouseX);
		paddle.Draw(screen);


		for (int i = 0; i < BRICK_ROWS; i++)
		{
			for (int j = 0; j < BRICK_COLUMNS; j++)
			{
				bricks[i][j].Draw(screen,
					OFFSET_X + j * (Brick::BRICK_WIDTH + GAP),
					i * (Brick::BRICK_HEIGHT + GAP));
			}
		}
	}
};