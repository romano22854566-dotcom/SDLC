#pragma once

#include <string>
#include <vector>

namespace habitflow {

/**
 * @brief Суточная запись выполнения привычек (Daily Habit Record)
 */
struct HabitRecord {
    std::string date;           ///< Календарная дата в формате ISO 8601 (YYYY-MM-DD)
    double sleepHours = 0.0;     ///< Фактическая продолжительность сна (в часах, 0.0 - 24.0)
    int mealsCount = 0;         ///< Количество выполненных основных приемов пищи (0 - 5)
    int waterMl = 0;            ///< Объем выпитой чистой воды (в миллилитрах, 0 - 10000)
    int walkMinutes = 0;        ///< Время активной прогулки на свежем воздухе (в минутах)
    int breakIntervals = 0;     ///< Количество выполненных 10-минутных пауз отдыха по таймеру 50/10
    
    double calculatedScore = 0.0; ///< Вычисленный суточный процент интегральной дисциплины (0.0 - 100.0%)
    bool isCompleted = false;     ///< Флаг успешного закрытия дня (calculatedScore >= 80.0%)
};

} // namespace habitflow
