#pragma once
#include "surface.h"

class Ball
{
public:
	void Move(bool bounceBottom, float deltaTime);

	void Draw(Tmpl8::Surface* surface);
	int GetX();
	int GetY();
	void SetX(int value);
	void SetY(int value);
	float GetDX();
	float GetDY();
	void SetDX(float value);
	void SetDY(float value);
	bool IsInPlay();
	void SetInPlay(bool value);
	float GetVelocity() const;

	Ball()
	{
		x = 400;
		y = 465;
		dx = 0;
		dy = 0;
		inPlay = false;
	}

private:
	static constexpr float velocity = 750.0f; // set the ball's velocity in pixels per second
	int x, y;
	float dx, dy;
	bool inPlay;
};

