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
        color = RED;
        break;
    case WIDE_PADDLE:
        color = BLUE;
        break;
    case MULTI_BALL:
        color = YELLOW;
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

bool Brick::IsDestroyed()
{
    return destroyed;
}

void Brick::SetDestroyed(bool value)
{
    destroyed = value;
}