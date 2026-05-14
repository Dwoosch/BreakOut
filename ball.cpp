#include "ball.h"
#include "GameState.h"
#include "template.h"

void Ball::Move(bool bounceBottom) 
{
	x += dx;
	y += dy;

	// bounce off the walls
	if (x < 0 || x > ScreenWidth)
	{
		dx = -dx;
		x = x < 0 ? 0 : ScreenWidth - 1;
		ma_engine_play_sound(&engine, "assets/ballbounce.mp3", NULL);
	}
	if (y < 0 || (bounceBottom && y > ScreenHeight))
	{
		y = y < 0 ? 0 : ScreenHeight - 1;
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