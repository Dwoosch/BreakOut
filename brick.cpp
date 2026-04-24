#include "brick.h"
#include "surface.h"

void Brick::Draw(Tmpl8::Surface* surface, int x, int y)
{
    if (!destroyed)
    {
        surface->Bar(x, y, x + BRICK_WIDTH - 1, y + BRICK_HEIGHT - 1, color);
    }
}