#pragma once
#include "surface.h"

class Ball
{
	enum Shapes { CIRCLE, SQUARE, TRIANGLE };

public:
	float x, y;
	float dx, dy;
	bool inPlay; // indicates whether the ball is currently in play
	Shapes shape;
	void Move(bool bounceBottom);

	void Draw(Tmpl8::Surface* surface);
	int GetVelocity() const;

	Ball()
	{
		x = 400;
		y = 465;
		dx = 0;
		dy = 0;
		shape = CIRCLE;
		inPlay = false;
	}

private:
	const float velocity = 5.0f; // set the ball's velocity
};

