#include "GameScene.h"
#include "brick.h"
#include "GameState.h"
#include "template.h"
#include "surface.h"
#include <cstdio> //printf
#include "ball.h"
#include "paddle.h"
#include "ParticleSystem.h"
#include <cmath>
#include <SDL_stdinc.h>
#include <algorithm>
#include <array>
#include <cstdlib>

GameState currentState = AIMING;
float widePaddleTimer = 0.0f;
bool widePaddleActive = false;
int lives = 3;
int score = 0;
float shakeIntensity = 0.0f;
Tmpl8::Surface* backBuffer;

std::array<Ball, MAX_BALLS> balls;
Paddle paddle;
std::array<std::array<Brick, BRICK_COLUMNS>, BRICK_ROWS> bricks;
ParticleSystem particleSystem;

// -----------------------------------------------------------
// Handle mouse input
// -----------------------------------------------------------
void GameScene::MouseMoveAbsolute(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

void GameScene::MouseDown(int button)
{
	if (button == 1 && currentState == AIMING) // 1 = left mouse button
	{
		currentState = PLAYING;
	}
}

// -----------------------------------------------------------
// Handle aiming state
// -----------------------------------------------------------
void GameScene::HandleAimingState()
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

		balls[0].dx = dirX * balls[0].GetVelocity();
		balls[0].dy = dirY * balls[0].GetVelocity();

		// draw aiming line
		DrawTrajectory(balls[0].x, balls[0].y, dirX, dirY);
	}
}

// -----------------------------------------------------------
// Drawing the ball's trajectory
// -----------------------------------------------------------
void GameScene::DrawTrajectory(float startX, float startY, float dirX, float dirY)
{
	// calculate intersection with walls
	float t1 = (ScreenWidth - startX) / dirX;
	float t2 = -startX / dirX;
	float t3 = -startY / dirY;

	// find the smallest positive t to determine which wall is hit first
	float t = FLT_MAX;
	if (t1 > 0) t = std::min(t, t1);
	if (t2 > 0) t = std::min(t, t2);
	if (t3 > 0) t = std::min(t, t3);

	// calculate the hit point
	float hitX = startX + dirX * t;
	float hitY = startY + dirY * t;
	backBuffer->Line(startX, startY, hitX, hitY, 0xFFFFFF);

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
	backBuffer->Line(hitX, hitY, hitX2, hitY2, 0xFFFFFF);
}

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void GameScene::Init()
{
	printf("GameScene Init called\n");
	// reset
	score = 0;
	lives = 3;
	currentState = AIMING;
	shakeIntensity = 0.0f;
	widePaddleActive = false;
	widePaddleTimer = 0.0f;
	// create a back buffer surface for screen shake effect
	backBuffer = new Tmpl8::Surface(ScreenWidth, ScreenHeight);
	// initialize the font for drawing text
	backBuffer->InitCharset();
	// set the first ball to be in play and reset the grid to initialize bricks and powerups
	balls[0].inPlay = true;
	ResetGrid();
}

// -----------------------------------------------------------
// Close down application
// -----------------------------------------------------------
void GameScene::Shutdown()
{
	// clean up back buffer surface
	delete backBuffer;
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------
void GameScene::Tick(float deltaTime)
{
	screen->Clear(0); // clear the screen to black
	backBuffer->Clear(0); // clear the back buffer to black
	// calculate shake offset
	int shakeX = 0, shakeY = 0;
	if (shakeIntensity > 0)
	{
		// shake intensity determines the maximum offset in pixels, 
		// we can use a random value between -shakeIntensity and +shakeIntensity for both x and y
		// moreover we clamp the minimum value passed to rand to 1 to avoid modulo by zero when shakeIntensity is very low
		int range = std::max(1, (int)(shakeIntensity * 2));
		shakeX = (rand() % range) - (int)shakeIntensity;
		shakeY = (rand() % range) - (int)shakeIntensity;
		shakeIntensity -= deltaTime * 0.005f;
		if (shakeIntensity < 0)
			shakeIntensity = 0;
	}

	char buffer[32];
	sprintf(buffer, "Score: %d", score);
	backBuffer->Print(buffer, 10, 10, 0xFFFFFF);
	sprintf(buffer, "Lives: %d", lives);
	backBuffer->Print(buffer, 750, 10, 0xFFFFFF);
	paddle.Draw(backBuffer);
	for (int i = 0; i < MAX_BALLS; i++)
	{
		if (balls[i].inPlay)
		{
			balls[i].Draw(backBuffer);
		}
	}

	for (int i = 0; i < BRICK_ROWS; i++)
	{
		for (int j = 0; j < BRICK_COLUMNS; j++)
		{
			bricks[i][j].Draw(backBuffer,
				OFFSET_X + j * (Brick::BRICK_WIDTH + GAP),
				i * (Brick::BRICK_HEIGHT + GAP));
		}
	}
	particleSystem.Update(deltaTime);
	particleSystem.Draw(backBuffer);

	switch (currentState)
	{
	case AIMING:
		backBuffer->Centre("Click to Aim", ScreenHeight / 2, 0xFFFFFF);
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
				balls[i].Move(false);
			}
		}

		HandleBrickCollisions();
		if (currentState != PLAYING)
			break;
		HandlePaddleCollision();

		// Check if the ball has fallen below the screen
		for (int i = 0; i < MAX_BALLS; i++)
		{
			if (balls[i].inPlay && balls[i].y > ScreenHeight)
			{
				float lastX = balls[i].x;
				float lastY = balls[i].y;
				balls[i].inPlay = false;
				// When a ball is lost emit a white explosion particle effect at the ball's last position
				particleSystem.Emit(20, lastX, ScreenHeight - 5, 1000.0f, 0xFFFFFF);
			}
		}
		if (AllBallsLost())
		{
			lives--;
			if (lives > 0)
			{
				shakeIntensity = 5.0f; // set shake intensity for screen shake effect
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
		nextScene = targetScene; // automatically transition to next scene
		break;
	}

	// copy backbuffer to screen with shake offset
	// CopyTo handles bounds internally 
	// so we don't need to worry about negative offsets or offsets that go beyond the screen dimensions
	backBuffer->CopyTo(screen, shakeX, shakeY);
}