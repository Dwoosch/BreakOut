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
	enum GameState { AIMING, PLAYING, GAMEOVER };
	GameState currentState = AIMING;

	// Constants for the brick grid
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

	void Game::MouseDown(int button)
	{
		if (button == 1 && currentState == AIMING) // 1 = left mouse button
		{
			currentState = PLAYING;
		}
	}

	// -----------------------------------------------------------
	// Drawing the ball's trajectory
	// -----------------------------------------------------------
	void Game::DrawTrajectory(float startX, float startY, float dirX, float dirY)
	{
		float t1 = (ScreenWidth - startX) / dirX;
		float t2 = -startX / dirX;
		float t3 = -startY / dirY;

		float t = FLT_MAX; // start with largest possible value
		if (t1 > 0) t = std::min(t, t1);
		if (t2 > 0) t = std::min(t, t2);
		if (t3 > 0) t = std::min(t, t3);

		float hitX = startX + dirX * t;
		float hitY = startY + dirY * t;
		screen->Line(startX, startY, hitX, hitY, 0xFFFFFF);

		if (t == t1 || t == t2) dirX = -dirX; // hit left or right wall
		if (t == t3) dirY = -dirY; // hit top wall

		t1 = (ScreenWidth - hitX) / dirX;
		t2 = -hitX / dirX;
		t3 = -hitY / dirY;

		float T = FLT_MAX;
		if (t1 > 0) T = std::min(T, t1);
		if (t2 > 0) T = std::min(T, t2);
		if (t3 > 0) T = std::min(T, t3);
		float hitX2 = hitX + dirX * T;
		float hitY2 = hitY + dirY * T;
		screen->Line(hitX, hitY, hitX2, hitY2, 0xFFFFFF);
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
		paddle.Draw(screen);
		ball.Draw(screen);

		for (int i = 0; i < BRICK_ROWS; i++)
		{
			for (int j = 0; j < BRICK_COLUMNS; j++)
			{
				bricks[i][j].Draw(screen,
					OFFSET_X + j * (Brick::BRICK_WIDTH + GAP),
					i * (Brick::BRICK_HEIGHT + GAP));
			}
		}

		if (currentState == AIMING)
		{
			ball.x = paddle.x + paddle.width / 2;
			ball.y = paddle.y - 10;

			if (mouseY < paddle.y)
			{
				float dirX = mouseX - ball.x;
				float dirY = mouseY - ball.y;

				printf("dirX: %f, dirY: %f\n", dirX, dirY);

				float length = sqrt(dirX * dirX + dirY * dirY);
				dirX /= length;
				dirY /= length;

				printf("normDirX: %f, normDirY: %f\n", dirX, dirY);

				ball.dx = dirX * ball.velocity;
				ball.dy = dirY * ball.velocity;

				// draw aiming line
				DrawTrajectory(ball.x, ball.y, dirX, dirY);
			}
		}

		if (currentState == PLAYING)	
		{
			paddle.Move(mouseX);
			ball.Move();

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
				// hit position normalized to range [-1, 1]
				float hitPos = (ball.x - (paddle.x + paddle.width / 2)) / ((float)paddle.width / 2);

				// convert degrees to radians for the maximum angle
				float maxAngle = 60 * (M_PI / 180);

				float angle = hitPos * maxAngle;

				ball.dx = ball.velocity * sin(angle);
				// ensure the ball always moves upwards after hitting the paddle
				if (ball.dy > 0)
				{
					ball.dy = -ball.velocity * cos(angle);
				}
			}
		}
	}
};