#pragma once

#include "habit_record.h"
#include "user_settings.h"

namespace habitflow {

/**
 * @brief Калькулятор интегрального суточного процента дисциплины
 */
class DisciplineCalculator {
public:
    DisciplineCalculator() = default;

    /**
     * @brief Расчет процента суточной дисциплины по взвешенной формуле 5 норм
     * @param record Фактические показатели суточной записи
     * @param settings Индивидуальные целевые настройки пользователя
     * @return Процент дисциплины от 0.0 до 100.0%
     */
    static double calculateDailyScore(const HabitRecord& record, const UserSettings& settings);

    /**
     * @brief Проверка соблюдения порога успешного дня (>= 80%)
     */
    static bool isDaySuccessful(double score, const UserSettings& settings);
};

} // namespace habitflow
