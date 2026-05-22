#ifndef MASCOT_HPP
#define MASCOT_HPP

namespace noonoo {

enum class MascotDirection { Down, Left, Right, Up };

class Mascot {
public:
    Mascot();

    void Update(float dt, float screenWidth, float screenHeight);
    void ToggleVisible();
    bool            IsVisible()     const;
    float           GetX()          const;
    float           GetY()          const;
    int             GetFrameIndex() const;
    MascotDirection GetDirection()  const;

private:
    void pickNewTarget(float screenWidth, float screenHeight);

    static constexpr int   FRAME_COUNT      = 4;
    static constexpr float FRAME_INTERVAL   = 0.12f;
    static constexpr float SPEED            = 80.0f;
    static constexpr float ARRIVE_THRESHOLD = 6.0f;

public:
    static constexpr float DISPLAY_SIZE = 32.0f;

private:

    float           _x            = 100.0f;
    float           _y            = 100.0f;
    float           _targetX      = 200.0f;
    float           _targetY      = 200.0f;
    float           _frameTimer   = 0.0f;
    int             _frameIndex   = 0;
    MascotDirection _direction    = MascotDirection::Right;
    bool            _visible      = true;
};

} // namespace noonoo

#endif // MASCOT_HPP
