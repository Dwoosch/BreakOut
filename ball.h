#pragma once
class Surface;
class Ball
{
	enum Shapes { CIRCLE, SQUARE, TRIANGLE };

public:
	int x, y;
	int dx, dy;
	Shapes shape;
	void Move()
	{
		// move the ball by dx and dy, bouncing off the walls
	}
	void Draw(Surface* surface)
	{
		// draw the ball at (x, y) with shape determined by dx and dy
	}
	Ball()
	{
		x = 200; // set the ball's initial position
		y = 300;
		dx = 0; // set the ball's initial velocity
		dy = 0;
		shape = CIRCLE; // set the ball's shape based on its velocity
	}
};

