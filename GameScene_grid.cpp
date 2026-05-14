#include "GameScene.h"

// -----------------------------------------------------------
// Check if all bricks are destroyed
// -----------------------------------------------------------
bool GameScene::AllBricksDestroyed()
{
	// loop through all bricks and check if any are not destroyed
	for (int i = 0; i < BRICK_ROWS; i++)
	{
		for (int j = 0; j < BRICK_COLUMNS; j++)
		{
			if (!bricks[i][j].IsDestroyed())
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
bool GameScene::AllBallsLost()
{
	// loop through all balls and check if any are still in play
	for (int i = 0; i < MAX_BALLS; i++)
	{
		if (balls[i].IsInPlay())
		{
			return false;
		}
	}
	return true;
}

// -----------------------------------------------------------
// Reset the brick grid
// -----------------------------------------------------------
void GameScene::ResetGrid()
{
	currentState = AIMING;
	ResetBalls();
	widePaddleActive = false;
	for (int row = 0; row < BRICK_ROWS; row++)
	{
		for (int col = 0; col < BRICK_COLUMNS; col++)
		{
			int powerUpNumber = IRand(9);
			bricks[row][col].SetDestroyed(false);
			// 60% chance of no powerup
			if (powerUpNumber < 6) bricks[row][col].SetPowerup(Brick::NONE);
			// 20% chance of wide paddle
			else if (powerUpNumber < 8) bricks[row][col].SetPowerup(Brick::WIDE_PADDLE);
			// 10% chance of multi-ball
			else bricks[row][col].SetPowerup(Brick::MULTI_BALL);
		}
	}
}

// -----------------------------------------------------------
// Reset the balls to their initial state
// -----------------------------------------------------------
void GameScene::ResetBalls()
{
	balls[0].SetInPlay(true);
	for (int i = 1; i < MAX_BALLS; i++)
	{
		balls[i].SetInPlay(false);
	}
}