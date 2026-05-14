#include "ball.h"
#include "template.h"
#include "Audio.h"

void Ball::Move(bool bounceBottom, float deltaTime)
{
	float dt = deltaTime / 1000.0f; // convert to seconds
	x += (int)(dx * dt);
	y += (int)(dy * dt);

	// bounce off the walls
	if (x < 0 || x > ScreenWidth)
	{
		dx = -dx;
		x = x < 0 ? 0 : ScreenWidth - 1;
		Audio::Play("assets/ballbounce.mp3");
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

int Ball::GetX()
{
	return (int)x;
}

int Ball::GetY()
{
	return (int)y;
}

void Ball::SetX(int value)
{
	x = value;
}

void Ball::SetY(int value)
{
	y = value;
}

float Ball::GetDX()
{
	return dx;
}

float Ball::GetDY()
{
	return dy;
}

void Ball::SetDX(float value)
{
	dx = value;
}

void Ball::SetDY(float value)
{
	dy = value;
}

bool Ball::IsInPlay()
{
	return inPlay;
}

void Ball::SetInPlay(bool value)
{
	inPlay = value;
}

float Ball::GetVelocity() const
{
	return velocity;
}