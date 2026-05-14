#pragma once
#include "surface.h"
class Paddle
{
public:
	void Move(int mouseX);
	void Draw(Tmpl8::Surface* surface);
	int GetWidth();
	void SetWidth(int newWidth);
	int GetYPosition();
	int GetXPosition();
	int GetHeight();
	Paddle() = default;
private:
	int x = 400 - width / 2;
	int width = 128;
	const int y = 475;
	const int height = 15;
};