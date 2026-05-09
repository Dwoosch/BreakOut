#pragma once
#include "brick.h"

namespace Tmpl8 {

class Surface;
class Game
{
public:
	int mouseX, mouseY;

	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown(int button);
	void MouseMove(int x, int  y) { /* implement if you want to detect mouse movement */ }
	void MouseMoveAbsolute(int x, int y);
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
	void ScreenShake(float intensity);
	void KeyUp( int key ) { /* implement if you want to handle keys */ }
	void KeyDown( int key ) { /* implement if you want to handle keys */ }
private:
	Surface* screen;
};

}; // namespace Tmpl8