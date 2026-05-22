#include "PlayerLevel.hpp"

namespace noonoo {

// ordered by ascending threshold — each entry maps to level (index + 1)
static constexpr LevelConfig LEVELS[PlayerLevel::MAX_LEVEL] = {
    {  0, "NinjaGreen",  BadgeShape::Circle   },
    { 10, "NinjaBlue",   BadgeShape::Triangle  },
    { 25, "NinjaRed",    BadgeShape::Diamond   },
    { 50, "NinjaYellow", BadgeShape::Pentagon  },
};

int PlayerLevel::Compute(int totalCorrect)
{
    int level = 1;
    for (int i = 1; i < MAX_LEVEL; i++) {
        if (totalCorrect >= LEVELS[i].threshold)
            level = i + 1;
    }
    return level;
}

LevelConfig PlayerLevel::ConfigFor(int level)
{
    int idx = level - 1;
    if (idx < 0)         idx = 0;
    if (idx >= MAX_LEVEL) idx = MAX_LEVEL - 1;
    return LEVELS[idx];
}

} // namespace noonoo
