#pragma once
#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "ParticleSystem.h"
#include "surface.h"
#include <array>
#include "template.h"

enum GameState { AIMING, PLAYING, GAMEOVER };

// Constants
const int BRICK_ROWS = 8;
const int BRICK_COLUMNS = 8;
const int GAP = 2; // pixels between bricks
const int GRID_WIDTH = BRICK_ROWS * (Brick::BRICK_WIDTH + GAP) - GAP; // subtract trailing gap
const int OFFSET_X = (ScreenWidth - GRID_WIDTH) / 2;
const int MAX_BALLS = 6;
const float WIDE_PADDLE_MULTIPLIER = 1.5f;

extern GameState currentState;
extern float widePaddleTimer;
extern bool widePaddleActive;
extern int lives;
extern int score;
extern float shakeIntensity;
extern Tmpl8::Surface* backBuffer;
extern std::array<Ball, MAX_BALLS> balls;
extern Paddle paddle;
extern std::array<std::array<Brick, BRICK_COLUMNS>, BRICK_ROWS> bricks;
extern ParticleSystem particleSystem;