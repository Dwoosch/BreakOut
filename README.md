# Breakout — BUAs CMGT Programming Intake 2026

A Breakout clone built from scratch in C++ using the Tmpl8 framework provided by BUAs.

## How to Build
Open `tmpl_2019-08.sln` in Visual Studio, set configuration to Release x64, and build. 
Requires the DLLs in `dlls_x64/` to be in the same folder as the executable.

## How to Play
- Move the mouse to aim your ball's trajectory, a preview line shows exactly where it will bounce
- Click to launch the ball
- Move the mouse to control the paddle
- Break all bricks to advance, reach the highest score possible

## Powerups
- 🔵 **Blue bricks** — Wide Paddle (paddle doubles in size for 10 seconds)
- 🟡 **Yellow bricks** — Multi Ball (spawns an additional ball)
- 🔴 **Red bricks** — No powerup

## Technical Highlights

### Ray Casting Trajectory System
Before launching, the ball's path is previewed using ray casting. Given a normalised 
direction vector (calculated from mouse position relative to the ball), the system 
calculates the parametric value `t` for each wall intersection using equations like 
`t = (ScreenWidth - x) / dirX`. The smallest positive `t` determines which wall is 
hit first, and the exact hit point is calculated as `start + direction * t`. After 
reflecting the direction vector off the wall (negating the appropriate component), 
the process repeats for the second bounce segment.

### Physics-Based Paddle Bounce
When the ball hits the paddle, the hit position is normalised to the range [-1, 1] 
by calculating `(ballX - paddleCenterX) / (paddleWidth / 2)`. This value is mapped 
to an angle in radians (capped at ±60 degrees to prevent horizontal shots). The new 
velocity is then `dx = velocity * sin(angle)`, `dy = -velocity * cos(angle)` using 
sin for horizontal and cos for vertical because at angle 0 (center hit), sin(0) = 0 
and cos(0) = 1, producing a straight upward bounce.

### 2D Rotation Matrix (Multiball)
When a multiball powerup activates, the new ball's direction is the source ball's 
direction rotated by 15 degrees using the standard 2D rotation matrix:
`newDx = dx * cos(θ) - dy * sin(θ)`
`newDy = dx * sin(θ) + dy * cos(θ)`
This guarantees the balls always diverge regardless of the source direction, 
including the edge case where dx is 0.

### AABB Collision Detection
Brick collisions use Axis-Aligned Bounding Box detection. To determine which side 
was hit, the overlap on each axis is calculated — for example 
`overlapX = min(ballRight - brickLeft, brickRight - ballLeft)`. The smaller overlap 
indicates the axis of least penetration, which is the side the ball entered from. 
This prevents incorrect bounces on corner hits.

### Back Buffer and Screen Shake
All rendering goes to a back buffer surface rather than directly to the screen. 
Each frame, a random offset scaled by `shakeIntensity` is calculated and passed to 
`backBuffer.CopyTo(screen, shakeX, shakeY)`. Since `CopyTo` handles out-of-bounds 
internally, this is completely safe. `shakeIntensity` decays each frame producing 
a natural falloff effect.

### Scene Manager Architecture
`Game` is a pure scene manager it holds a `currentScene` pointer and delegates 
all `Tick`, `Init`, `Shutdown` and input calls to it. Scenes signal a transition 
by setting their `nextScene` pointer, which `Game::Tick` checks after each update. 
This allows clean separation between `MenuScene`, `GameScene` and `GameOverScene` 
with no coupling between scenes themselves.

### Custom Surface Extensions
Two functions were added to the Tmpl8 `Surface` class:
- `Circle(x, y, radius, color)` draws a filled circle using the optimised 
  distance-squared check `(dx*dx + dy*dy <= r*r)`, avoiding the costly `sqrt` 
  operation
- `ScalePixelColor(pixel, weight)` — scales each RGB channel independently by a 
  float weight, used for fading particles over their lifetime
## Dependencies
- SDL2 2.0.3
- miniaudio (single header, included)
