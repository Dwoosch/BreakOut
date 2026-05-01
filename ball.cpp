#include "ball.h"

void Ball::Move() 
{
	x += dx;
	y += dy;

	// bounce off the walls
	if (x < 0 || x > 800)
	{
		dx = -dx;
	}
	if (y < 0 || y > 512)
	{
		dy = -dy;
	}
}

void Ball::Draw(Tmpl8::Surface* surface) 
{
	surface->Circle(x, y, 10, 0xFFFFFF);
}