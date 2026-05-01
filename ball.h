#pragma once
#include "surface.h"

class Ball
{
	enum Shapes { CIRCLE, SQUARE, TRIANGLE };

public:
	int x, y;
	int dx, dy;
	const int velocity = 5; // set the ball's velocity
	Shapes shape;
	void Move();

	void Draw(Tmpl8::Surface* surface);

	Ball()
	{
		x = 200;
		y = 300;
		dx = velocity;
		dy = velocity;
		shape = CIRCLE;
	}
};

