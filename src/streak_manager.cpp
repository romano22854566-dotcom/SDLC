#include "streak_manager.h"
#include <algorithm>

namespace habitflow {

StreakInfo StreakManager::evaluateStreaks(const std::vector<HabitRecord>& history, double threshold) {
    StreakInfo info;
    if (history.empty()) {
        return info;
    }

    int current = 0;
    int maxStreak = 0;

    for (const auto& rec : history) {
        if (rec.calculatedScore >= threshold) {
            current++;
            maxStreak = std::max(maxStreak, current);
        } else {
            current = 0; // Серия прервана
        }
    }

    info.currentStreak = current;
    info.longestStreak = maxStreak;
    info.isStreakActive = (current > 0);

    return info;
}

} // namespace habitflow
