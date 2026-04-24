#pragma once
#include "surface.h"

class Brick
{
public:
	static const int BRICK_WIDTH = 64;
	static const int BRICK_HEIGHT = 32;

	bool destroyed;

	enum PowerupType { NONE, WIDE_PADDLE, MULTI_BALL };

	PowerupType powerup;
	Tmpl8::Pixel color;
	
	void Draw(Tmpl8::Surface* surface, int x, int y);

	Brick()
	{
		destroyed = false; // set the brick's initial state to not destroyed
		powerup = NONE; // set the brick's initial powerup type to none
		color = 0xFF0000; // set the brick's initial color to red
	}
};

