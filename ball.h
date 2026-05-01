#pragma once
#include "surface.h"

class Ball
{
	enum Shapes { CIRCLE, SQUARE, TRIANGLE };

public:
	float x, y;
	float dx, dy;
	const float velocity = 5.0f; // set the ball's velocity
	Shapes shape;
	void Move();

	void Draw(Tmpl8::Surface* surface);

	Ball()
	{
		x = 400;
		y = 465;
		dx = 0;
		dy = 0;
		shape = CIRCLE;
	}
};

