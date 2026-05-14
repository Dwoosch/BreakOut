#pragma once
#include "surface.h"

class Brick
{
public:
	static const int BRICK_WIDTH = 64;
	static const int BRICK_HEIGHT = 32;
	enum PowerupType { NONE, WIDE_PADDLE, MULTI_BALL };
	enum Color { RED = 0xFF0000, BLUE = 0x0000FF, YELLOW = 0xFFFF00 };

	void Draw(Tmpl8::Surface* surface, int x, int y);
	void SetPowerup(PowerupType type);
	PowerupType GetPowerupType() const;
	Tmpl8::Pixel GetColor() const;
	bool IsDestroyed();
	void SetDestroyed(bool value);

	Brick() = default;
private:
	bool destroyed = false;
	Tmpl8::Pixel color = RED;
	PowerupType powerup = NONE;
};