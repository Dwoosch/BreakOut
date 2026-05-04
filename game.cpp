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
		float widePaddleTimer = 0.0f;
		bool widePaddleActive = false;
		int lives = 3;
		const int MAX_BALLS = 6;

		// Constants for the brick grid
		const int BRICK_ROWS = 8;
		const int BRICK_COLUMNS = 8;
		const int GAP = 2; // pixels between bricks
		const int GRID_WIDTH = BRICK_ROWS * (Brick::BRICK_WIDTH + GAP) - GAP; // subtract trailing gap
		const int OFFSET_X = (ScreenWidth - GRID_WIDTH) / 2;

		Ball balls[MAX_BALLS];
		Paddle paddle;
		Brick bricks[BRICK_ROWS][BRICK_COLUMNS];


		// -----------------------------------------------------------
		// Check if all bricks are destroyed
		// -----------------------------------------------------------
		bool Game::AllBricksDestroyed()
		{
			for (int i = 0; i < BRICK_ROWS; i++)
			{
				for (int j = 0; j < BRICK_COLUMNS; j++)
				{
					if (!bricks[i][j].destroyed)
					{
						return false;
					}
				}
			}
			return true;
		}

		// -----------------------------------------------------------
		// Check if all balls are lost
		// -----------------------------------------------------------
		bool Game::AllBallsLost()
		{
			for (int i = 0; i < MAX_BALLS; i++)
			{
				if (balls[i].inPlay)
				{
					return false;
				}
			}
			return true;
		}

		// -----------------------------------------------------------
		// Reset the brick grid
		// -----------------------------------------------------------
		void Game::ResetGrid()
		{
			currentState = AIMING;
			ResetBalls();
			widePaddleActive = false;
			for (int row = 0; row < BRICK_ROWS; row++)
			{
				for (int col = 0; col < BRICK_COLUMNS; col++)
				{
					int powerUpNumber = IRand(9);
					bricks[row][col].destroyed = false;
					// 60% chance of no powerup
					if (powerUpNumber < 6) bricks[row][col].powerup = Brick::NONE;
					// 20% chance of wide paddle
					else if (powerUpNumber < 8) bricks[row][col].powerup = Brick::WIDE_PADDLE;
					// 10% chance of multi-ball
					else bricks[row][col].powerup = Brick::MULTI_BALL;


					switch (bricks[row][col].powerup)
					{
					case Brick::PowerupType::NONE:
						bricks[row][col].color = 0xFF0000; // red
						break;
					case Brick::PowerupType::WIDE_PADDLE:
						bricks[row][col].color = 0x0000FF; // blue
						break;
					case Brick::PowerupType::MULTI_BALL:
						bricks[row][col].color = 0xFFFF00; // yellow
						break;
					}
				}
			}
		}

		// -----------------------------------------------------------
		// Reset the balls to their initial state
		// -----------------------------------------------------------
		void Game::ResetBalls()
		{
			balls[0].inPlay = true;
			for (int i = 1; i < MAX_BALLS; i++)
			{
				balls[i].inPlay = false;
			}
		}

		// -----------------------------------------------------------
		// Handle mouse input
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
		// Handle aiming state
		// -----------------------------------------------------------
		void Game::HandleAimingState()
		{
			balls[0].x = paddle.x + paddle.width / 2;
			balls[0].y = paddle.y - 10;

			if (mouseY < paddle.y)
			{
				float dirX = mouseX - balls[0].x;
				float dirY = mouseY - balls[0].y;

				float length = sqrt(dirX * dirX + dirY * dirY);
				dirX /= length;
				dirY /= length;

				balls[0].dx = dirX * balls[0].velocity;
				balls[0].dy = dirY * balls[0].velocity;

				// draw aiming line
				DrawTrajectory(balls[0].x, balls[0].y, dirX, dirY);
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

			float t = FLT_MAX;
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
		// Handle powerup effects
		// -----------------------------------------------------------
		void Game::HandlePowerup(Brick::PowerupType powerup, float dx, float dy, float x, float y)
		{
			switch (powerup)
			{
				case Brick::WIDE_PADDLE:
					ActivateWidePaddlePowerup();
					break;
				case Brick::MULTI_BALL:
					ActivateMultiBallPowerup(dx, dy, x, y);
					break;
			}
		}

		// -----------------------------------------------------------
		// Handling collisions with bricks
		// -----------------------------------------------------------
		void Game::HandleBrickCollisions()
		{
			for (int i = 0; i < MAX_BALLS; i++)
			{
				if (balls[i].inPlay)
				{
					bool hit = false;
					for (int row = 0; row < BRICK_ROWS && !hit; row++)
					{
						for (int col = 0; col < BRICK_COLUMNS && !hit; col++)
						{
							if (!bricks[row][col].destroyed)
							{
								int brickX = OFFSET_X + col * (Brick::BRICK_WIDTH + GAP);
								int brickY = row * (Brick::BRICK_HEIGHT + GAP);
								if (CheckCollision2D(balls[i].x - 5, balls[i].y - 5, 10, 10, brickX, brickY, Brick::BRICK_WIDTH, Brick::BRICK_HEIGHT))
								{
									HandlePowerup(bricks[row][col].powerup, balls[i].dx, balls[i].dy, balls[i].x, balls[i].y);
									bricks[row][col].destroyed = true;
									int ballLeft = balls[i].x - 5;
									int ballRight = balls[i].x + 5;
									int ballTop = balls[i].y - 5;
									int ballBottom = balls[i].y + 5;

									int brickRight = brickX + Brick::BRICK_WIDTH;
									int brickBottom = brickY + Brick::BRICK_HEIGHT;

									int overlapX = std::min(ballRight - brickX, brickRight - ballLeft);
									int overlapY = std::min(ballBottom - brickY, brickBottom - ballTop);

									if (overlapX < overlapY)
									{
										balls[i].dx = -balls[i].dx; // reverse horizontal direction
									}
									else
									{
										balls[i].dy = -balls[i].dy; // reverse vertical direction
									}
									if (AllBricksDestroyed())
									{
										ResetGrid();
									}
									hit = true;
								}
							}
						}
					}
				}
			}
		}

		// -----------------------------------------------------------
		// Handling collision with the paddle
		// -----------------------------------------------------------
		void Game::HandlePaddleCollision()
		{
			for (int i = 0; i < MAX_BALLS; i++)
			{
				if (balls[i].inPlay)
				{
					if (CheckCollision2D(balls[i].x - 5, balls[i].y - 5, 10, 10, paddle.x, paddle.y, paddle.width, paddle.height))
					{
						// hit position normalized to range [-1, 1]
						float hitPos = (balls[i].x - (paddle.x + paddle.width / 2)) / ((float)paddle.width / 2);
						// convert degrees to radians for the maximum angle
						float maxAngle = 60 * (M_PI / 180);
						float angle = hitPos * maxAngle;
						balls[i].dx = balls[i].velocity * sin(angle);
						// ensure the ball always moves upwards after hitting the paddle
						if (balls[i].dy > 0)
						{
							balls[i].dy = -balls[i].velocity * cos(angle);
						}
					}
				}
			}
		}

		// -----------------------------------------------------------
		// Activate multi-ball powerup
		// -----------------------------------------------------------
		void Game::ActivateMultiBallPowerup(float sourceDx, float sourceDy, float sourceX, float sourceY)
		{
			for (int i = 1; i < MAX_BALLS; i++)
			{
				if (!balls[i].inPlay)
				{
					balls[i].inPlay = true;
					balls[i].x = sourceX;
					balls[i].y = sourceY;
					// Diverge the new ball's direction by 15 degrees from the original ball's direction
					float angle = 15 * (M_PI / 180); // convert degrees to radians
					float newDx = sourceDx * cos(angle) - sourceDy * sin(angle);
					float newDy = sourceDx * sin(angle) + sourceDy * cos(angle);
					balls[i].dx = newDx;
					balls[i].dy = newDy;
					break; // only activate one additional ball
				}
			}
		}

		// -----------------------------------------------------------
		// Activate wide paddle powerup
		// -----------------------------------------------------------
		void Game::ActivateWidePaddlePowerup()
		{
			// Increase paddle width by 50% for 10 seconds
			paddle.width = 192; // 128 * 1.5
			widePaddleActive = true;
			widePaddleTimer = 10000.0f; // reset timer

		}

		// -----------------------------------------------------------
		// Initialize the application
		// -----------------------------------------------------------
		void Game::Init()
		{
			balls[0].inPlay = true;
			ResetGrid();
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
			for (int i = 0; i < MAX_BALLS; i++)
			{
				if (balls[i].inPlay)
				{
					balls[i].Draw(screen);
				}
			}

			for (int i = 0; i < BRICK_ROWS; i++)
			{
				for (int j = 0; j < BRICK_COLUMNS; j++)
				{
					bricks[i][j].Draw(screen,
						OFFSET_X + j * (Brick::BRICK_WIDTH + GAP),
						i * (Brick::BRICK_HEIGHT + GAP));
				}
			}

			switch (currentState)
			{
				case AIMING:
					screen->Centre("Click to Aim", ScreenHeight / 2, 0xFFFFFF);
					HandleAimingState();
					break;
				case PLAYING:
					paddle.Move(mouseX);

					if (widePaddleActive)
					{
						widePaddleTimer -= deltaTime;
						if (widePaddleTimer <= 0)
						{
							paddle.width = 128; // reset to original width
							widePaddleActive = false;
						}
					}

					for (int i = 0; i < MAX_BALLS; i++)
					{
						if (balls[i].inPlay)
						{
							balls[i].Move();
						}
					}

					HandleBrickCollisions();
					HandlePaddleCollision();

					// Check if the ball has fallen below the screen
					for (int i = 0; i < MAX_BALLS; i++)
					{
						if (balls[i].inPlay && balls[i].y > ScreenHeight)
						{
							balls[i].inPlay = false;
						}
					}
					if (AllBallsLost())
					{
						lives--;
						if (lives > 0)
						{
							ResetBalls();
							currentState = AIMING;
						}
						else
						{
							currentState = GAMEOVER;
						}
					}
					break;
				case GAMEOVER:
					screen->Centre("Game Over", ScreenHeight / 2, 0xFFFFFF);
					break;

			}
		}
	};