#pragma once
#include "Scene.h"
#include "brick.h"
#include "ball.h"
#include "ParticleSystem.h"
#include "paddle.h"
#include "template.h"

class GameScene : public Scene
{
public:
	// Constants
	static const int BRICK_ROWS = 8;
	static const int BRICK_COLUMNS = 8;
	static const int GAP = 2; // pixels between bricks
	static const int GRID_WIDTH = BRICK_ROWS * (Brick::BRICK_WIDTH + GAP) - GAP; // subtract trailing gap
	static const int OFFSET_X = (ScreenWidth - GRID_WIDTH) / 2;
	static const int MAX_BALLS = 6;
	const float WIDE_PADDLE_MULTIPLIER = 1.5f;

	~GameScene() override;
	void Init() override;
	std::unique_ptr<Scene> Tick(float deltaTime) override;
	void Draw(Tmpl8::Surface* screen) override;
	void Shutdown() override;
	void MouseDown(int button) override;
	void MouseMove(int x, int y) override {/* implement if you want to detect mouse movement */ }
	void MouseMoveAbsolute(int x, int y) override;

	void DrawTrajectory(float startX, float startY, float dirX, float dirY);
	bool AllBricksDestroyed();
	bool AllBallsLost();
	void ResetGrid();
	void ResetBalls();
	bool CheckCollision2D(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh)
	{
		return (ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by);
	}
	void HandleAimingState();
	void HandlePowerup(Brick::PowerupType powerup, float dx, float dy, float x, float y);
	void HandleBrickCollisions();
	void HandlePaddleCollision();
	void ActivateMultiBallPowerup(float sourceDx, float sourceDy, float sourceX, float sourceY);
	void ActivateWidePaddlePowerup();

private:
	int shakeX = 0;
    int shakeY = 0;
	Tmpl8::Surface* backBuffer = nullptr;


	enum GameState { AIMING, PLAYING, GAMEOVER };

	GameState currentState = AIMING;
	float widePaddleTimer = 0.0f;
	bool widePaddleActive = false;
	int lives = 3;
	int score = 0;
	float shakeIntensity = 0.0f;
	ma_sound backgroundMusic;
	float musicDelay = 2000.0f;

	std::array<Ball, MAX_BALLS> balls;
	Paddle paddle;
	std::array<std::array<Brick, BRICK_COLUMNS>, BRICK_ROWS> bricks;
	ParticleSystem particleSystem;
};

