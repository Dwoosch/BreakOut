#include "GameScene.h"
#include "GameState.h"
#include <SDL_stdinc.h>

// -----------------------------------------------------------
// Handle powerup effects
// -----------------------------------------------------------
void GameScene::HandlePowerup(Brick::PowerupType powerup, float dx, float dy, float x, float y)
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
// Activate multi-ball powerup
// -----------------------------------------------------------
void GameScene::ActivateMultiBallPowerup(float sourceDx, float sourceDy, float sourceX, float sourceY)
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
void GameScene::ActivateWidePaddlePowerup()
{
	// Increase paddle width by 50% for 10 seconds
	paddle.width = (int)(128 * WIDE_PADDLE_MULTIPLIER); // 128 * 1.5
	widePaddleActive = true;
	widePaddleTimer = 10000.0f; // reset timer to 10 seconds (10000 milliseconds)

}