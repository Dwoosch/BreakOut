#pragma once
#include "Scene.h"
#include "brick.h"

class GameScene : public Scene
{
public:
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
};

