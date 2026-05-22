#ifndef PLAYER_LEVEL_HPP
#define PLAYER_LEVEL_HPP

namespace noonoo {

enum class BadgeShape { Circle, Triangle, Diamond, Pentagon };

struct LevelConfig {
    int         threshold;
    const char* ninjaFolder;
    BadgeShape  badge;
};

class PlayerLevel {
public:
    static constexpr int MAX_LEVEL = 4;

    static int         Compute(int totalCorrect);
    static LevelConfig ConfigFor(int level);
};

} // namespace noonoo

#endif // PLAYER_LEVEL_HPP
