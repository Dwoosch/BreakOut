#include "GameScene.h"
#include <SDL_stdinc.h>

// -----------------------------------------------------------
// Handling collisions with bricks
// -----------------------------------------------------------
void GameScene::HandleBrickCollisions()
{
	for (int i = 0; i < MAX_BALLS; i++)
	{
		if (!balls[i].IsInPlay())
			continue;

		bool hit = false;
		for (int row = 0; row < BRICK_ROWS && !hit; row++)
		{
			for (int col = 0; col < BRICK_COLUMNS && !hit; col++)
			{
				if (bricks[row][col].IsDestroyed())
					continue;

				int brickWidth = bricks[row][col].BRICK_WIDTH;
				int brickHeight = bricks[row][col].BRICK_HEIGHT;

				int brickX = OFFSET_X + col * (brickWidth + GAP);
				int brickY = row * (brickHeight + GAP);
				if (!CheckCollision2D(balls[i].GetX() - 5, balls[i].GetY() - 5, 10, 10, brickX, brickY, brickWidth, brickHeight))
					continue;

				HandlePowerup(bricks[row][col].GetPowerupType(), balls[i].GetDX(), balls[i].GetDY(), balls[i].GetX(), balls[i].GetY());
				bricks[row][col].SetDestroyed(true);
				shakeIntensity = 5.0f; // set shake intensity for screen shake effect
				Audio::Play("assets/collision.wav");
				particleSystem.Emit(20, balls[i].GetX(), balls[i].GetY(), 1000.0f, bricks[row][col].GetColor());
				score += 100;
				int ballLeft = balls[i].GetX() - 5;
				int ballRight = balls[i].GetX() + 5;
				int ballTop = balls[i].GetY() - 5;
				int ballBottom = balls[i].GetY() + 5;

				int brickRight = brickX + brickWidth;
				int brickBottom = brickY + brickHeight;

				// calculate overlap on both axes
				int overlapX = (std::min)(ballRight - brickX, brickRight - ballLeft);
				int overlapY = (std::min)(ballBottom - brickY, brickBottom - ballTop);

				// determine collision side based on smaller overlap
				if (overlapX < overlapY)
				{
					balls[i].SetDX(-balls[i].GetDX()); // reverse horizontal direction
				}
				else
				{
					balls[i].SetDY(-balls[i].GetDY()); // reverse vertical direction
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
		if (balls[i].IsInPlay())
		{
			if (GameScene::CheckCollision2D(balls[i].GetX() - 5, balls[i].GetY() - 5, 10, 10,
				paddle.GetXPosition(), paddle.GetYPosition(),
				paddle.GetWidth(), paddle.GetHeight()))
			{
				// hit position normalized to range [-1, 1]
				float hitPos = (balls[i].GetX() - (paddle.GetXPosition() + paddle.GetWidth() / 2)) / ((float)paddle.GetWidth() / 2);
				// convert degrees to radians for the maximum angle
				float maxAngle = 60 * (M_PI / 180);
				float angle = hitPos * maxAngle;
				balls[i].SetDX(balls[i].GetVelocity() * sin(angle));
				// ensure the ball always moves upwards after hitting the paddle
				if (balls[i].GetDY() > 0)
				{
					balls[i].SetDY(-balls[i].GetVelocity() * cos(angle));
				}
				Audio::Play("assets/ballbounce.mp3");
			}
		}
	}
}