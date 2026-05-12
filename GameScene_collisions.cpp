#include "GameScene.h"
#include <SDL_stdinc.h>
#include "GameState.h"

// -----------------------------------------------------------
// Handling collisions with bricks
// -----------------------------------------------------------
void GameScene::HandleBrickCollisions()
{
	for (int i = 0; i < MAX_BALLS; i++)
	{
		if (!balls[i].inPlay)
			continue;

		bool hit = false;
		for (int row = 0; row < BRICK_ROWS && !hit; row++)
		{
			for (int col = 0; col < BRICK_COLUMNS && !hit; col++)
			{
				if (bricks[row][col].destroyed)
					continue;

				int brickX = OFFSET_X + col * (Brick::BRICK_WIDTH + GAP);
				int brickY = row * (Brick::BRICK_HEIGHT + GAP);
				if (!CheckCollision2D(balls[i].x - 5, balls[i].y - 5, 10, 10, brickX, brickY, Brick::BRICK_WIDTH, Brick::BRICK_HEIGHT))
					continue;

				HandlePowerup(bricks[row][col].GetPowerupType(), balls[i].dx, balls[i].dy, balls[i].x, balls[i].y);
				bricks[row][col].destroyed = true;
				shakeIntensity = 5.0f; // set shake intensity for screen shake effect
				ma_engine_play_sound(&engine, "assets/collision.wav", NULL);
				particleSystem.Emit(20, balls[i].x, balls[i].y, 1000.0f, bricks[row][col].GetColor());
				score += 100;
				int ballLeft = balls[i].x - 5;
				int ballRight = balls[i].x + 5;
				int ballTop = balls[i].y - 5;
				int ballBottom = balls[i].y + 5;

				int brickRight = brickX + Brick::BRICK_WIDTH;
				int brickBottom = brickY + Brick::BRICK_HEIGHT;

				// calculate overlap on both axes
				int overlapX = (std::min)(ballRight - brickX, brickRight - ballLeft);
				int overlapY = (std::min)(ballBottom - brickY, brickBottom - ballTop);

				// determine collision side based on smaller overlap
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
					return;
				}
				hit = true;
			}
		}
	}
}

// -----------------------------------------------------------
// Handling collision with the paddle
// -----------------------------------------------------------
void GameScene::HandlePaddleCollision()
{
	for (int i = 0; i < MAX_BALLS; i++)
	{
		if (balls[i].inPlay)
		{
			if (GameScene::CheckCollision2D(balls[i].x - 5, balls[i].y - 5, 10, 10, paddle.x, paddle.y, paddle.width, paddle.height))
			{
				// hit position normalized to range [-1, 1]
				float hitPos = (balls[i].x - (paddle.x + paddle.width / 2)) / ((float)paddle.width / 2);
				// convert degrees to radians for the maximum angle
				float maxAngle = 60 * (M_PI / 180);
				float angle = hitPos * maxAngle;
				balls[i].dx = balls[i].GetVelocity() * sin(angle);
				// ensure the ball always moves upwards after hitting the paddle
				if (balls[i].dy > 0)
				{
					balls[i].dy = -balls[i].GetVelocity() * cos(angle);
				}
				ma_engine_play_sound(&engine, "assets/ballbounce.mp3", NULL);
			}
		}
	}
}