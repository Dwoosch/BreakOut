#include "brick.h"
#include "surface.h"

void Brick::Draw(Tmpl8::Surface* surface, int x, int y)
{
    if (!destroyed)
    {
        surface->Bar(x, y, x + BRICK_WIDTH - 1, y + BRICK_HEIGHT - 1, color);
    }
}   

void Brick::SetPowerup(PowerupType type)
{
    powerup = type;

	switch (powerup)
    {
        case NONE:
            color = RED; // red
            break;
        case WIDE_PADDLE:
            color = BLUE; // blue
            break;
        case MULTI_BALL:
            color = YELLOW; // yellow
            break;
    }
}

Brick::PowerupType Brick::GetPowerupType() const
{
    return powerup;
}

Tmpl8::Pixel Brick::GetColor() const
{
    return color;
}