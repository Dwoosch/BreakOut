#pragma once
class Surface;
class Paddle
{
public:
	Paddle() : y(450), height(15)
	{
		x = 400; // set the paddle's initial position
		dx = 0; // set the paddle's initial velocity
		width = 64; // set the paddle's initial width
	}
	int x;
	const int y;
	int dx;
	int width;
	const int height;
	
	void Move()
	{
		// move the paddle by dx, keeping it within the screen bounds
	}
	void Draw(Surface* surface)
	{
		// draw the paddle at (x, y) with the specified width and height
	}
};