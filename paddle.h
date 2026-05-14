#pragma once
#include "surface.h"
class Paddle
{
public:
	int width = 128;
	int x = 400 - width / 2;
	const int y = 475;
	const int height = 15;
	
	Paddle() = default;

	void Move(int mouseX);
	void Draw(Tmpl8::Surface* surface);
};