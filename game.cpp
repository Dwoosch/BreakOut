#include "game.h"
#include "template.h"
#include "surface.h"
#include <cstdio> //printf
#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include <cmath>
#include <SDL_stdinc.h>
#include <algorithm>

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
		ball.Draw(screen);
		ball.Move();

		for (int i = 0; i < BRICK_ROWS; i++)
		{
			for (int j = 0; j < BRICK_COLUMNS; j++)
			{
				bricks[i][j].Draw(screen,
					OFFSET_X + j * (Brick::BRICK_WIDTH + GAP),
					i * (Brick::BRICK_HEIGHT + GAP));
			}
		}

		for (int i = 0; i < BRICK_ROWS; i++)
		{
			for (int j = 0; j < BRICK_COLUMNS; j++)
			{
				if (!bricks[i][j].destroyed)
				{
					int brickX = OFFSET_X + j * (Brick::BRICK_WIDTH + GAP);
					int brickY = i * (Brick::BRICK_HEIGHT + GAP);
					if (CheckCollision2D(ball.x - 5, ball.y - 5, 10, 10, brickX, brickY, Brick::BRICK_WIDTH, Brick::BRICK_HEIGHT))
					{
						bricks[i][j].destroyed = true;
						int ballLeft = ball.x - 5;
						int ballRight = ball.x + 5;
						int ballTop = ball.y - 5;
						int ballBottom = ball.y + 5;

						int brickRight = brickX + Brick::BRICK_WIDTH;
						int brickBottom = brickY + Brick::BRICK_HEIGHT;

						int overlapX = std::min(ballRight - brickX, brickRight - ballLeft);
						int overlapY = std::min(ballBottom - brickY, brickBottom - ballTop);

						if (overlapX < overlapY)
						{
							ball.dx = -ball.dx; // reverse horizontal direction
						}
						else
						{
							ball.dy = -ball.dy; // reverse vertical direction
						}

						break; // only handle one collision per tick for simplicity
					}
				}
			}
		}

		if (CheckCollision2D(ball.x - 5, ball.y - 5, 10, 10, paddle.x, paddle.y, paddle.width, paddle.height))
		{
			float hitPos = (ball.x - (paddle.x + paddle.width / 2)) / ((float)paddle.width / 2);

			float maxAngle = 60 * (M_PI / 180);

			float angle = hitPos * maxAngle;

			ball.dx = ball.velocity * sin(angle);
			if (ball.dy > 0) 
			{
				ball.dy = -ball.velocity * cos(angle);
			}
		}
	}
};