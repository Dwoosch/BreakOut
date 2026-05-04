#include "paddle.h"
#include "surface.h"

void Paddle::Move(int mouseX)
{
	x = mouseX - width / 2;
	if (x < 0) x = 0; // prevent moving off the left edge
	if (x + width > 800) x = 800 - width; // prevent moving off the right edge
}

void Paddle::Draw(Tmpl8::Surface* surface)
{
	surface->Bar(x, y, x + width - 1, y + height - 1, 0x00FF00); // draw the paddle as a green bar
}