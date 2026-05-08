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
	if (y < 0)
	{
		dy = -dy;
	}
}

void Ball::Draw(Tmpl8::Surface* surface) 
{
	surface->Circle((int)x, (int)y, 10, 0xFFFFFF);
}

int Ball::GetVelocity() const 
{
	return velocity;
}