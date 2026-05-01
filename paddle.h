#pragma once
#include "surface.h"
class Paddle
{
public:
	int x;
	const int y;
	int width;
	const int height;
	
	void Move(int mouseX);
	void Draw(Tmpl8::Surface* surface);

	Paddle() : y(475), height(15)
	{
		width = 128; // set the paddle's initial width
		x = 400 - width / 2;
	}
};