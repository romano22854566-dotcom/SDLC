#pragma once

#include "habit_record.h"
#include <vector>

namespace habitflow {

struct StreakInfo {
    int currentStreak = 0;   ///< Текущая серия дней подряд с дисциплиной >= 80%
    int longestStreak = 0;   ///< Исторический рекорд серии за все время
    bool isStreakActive = false; ///< Активен ли стрик на текущую дату
};

/**
 * @brief Менеджер непрерывных серий выполнения привычек (Streak Manager)
 */
class StreakManager {
public:
    StreakManager() = default;

    /**
     * @brief Полный перерасчет стриков по хронологическому массиву исторических записей
     */
    static StreakInfo evaluateStreaks(const std::vector<HabitRecord>& history, double threshold = 80.0);
};

} // namespace habitflow
