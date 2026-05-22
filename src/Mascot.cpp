#include "Mascot.hpp"

#include <cmath>
#include <random>

namespace noonoo {

static std::mt19937& rng()
{
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

Mascot::Mascot()
{
    _targetX = _x;
    _targetY = _y;
}

void Mascot::pickNewTarget(float screenWidth, float screenHeight)
{
    std::uniform_real_distribution<float> distX(DISPLAY_SIZE, screenWidth  - DISPLAY_SIZE * 2.0f);
    std::uniform_real_distribution<float> distY(DISPLAY_SIZE, screenHeight - DISPLAY_SIZE * 2.0f);
    _targetX = distX(rng());
    _targetY = distY(rng());
}

void Mascot::Update(float dt, float screenWidth, float screenHeight)
{
    if (!_visible) return;

    float dx = _targetX - _x;
    float dy = _targetY - _y;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist < ARRIVE_THRESHOLD) {
        pickNewTarget(screenWidth, screenHeight);
        return;
    }

    float nx = dx / dist;
    float ny = dy / dist;
    _x += nx * SPEED * dt;
    _y += ny * SPEED * dt;

    if (std::abs(dx) >= std::abs(dy))
        _direction = dx > 0 ? MascotDirection::Right : MascotDirection::Left;
    else
        _direction = dy > 0 ? MascotDirection::Down : MascotDirection::Up;

    _frameTimer += dt;
    if (_frameTimer >= FRAME_INTERVAL) {
        _frameTimer -= FRAME_INTERVAL;
        _frameIndex = (_frameIndex + 1) % FRAME_COUNT;
    }
}

void Mascot::ToggleVisible()   { _visible = !_visible; }
bool Mascot::IsVisible()       const { return _visible; }
float Mascot::GetX()           const { return _x; }
float Mascot::GetY()           const { return _y; }
int   Mascot::GetFrameIndex()  const { return _frameIndex; }
MascotDirection Mascot::GetDirection() const { return _direction; }

} // namespace noonoo
