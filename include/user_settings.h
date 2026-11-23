#pragma once

namespace habitflow {

/**
 * @brief Пользовательские нормы и целевые параметры здорового распорядка дня
 */
struct UserSettings {
    double targetSleepHours = 8.0;   ///< Целевая продолжительность ночного сна (ч), по умолчанию 8.0
    int targetMealsCount = 3;         ///< Целевое количество основных приемов пищи, по умолчанию 3
    int targetWaterMl = 2000;         ///< Суточная норма чистой воды (мл), по умолчанию 2000 мл
    int targetWalkMinutes = 45;       ///< Целевое время активной прогулки (мин), по умолчанию 45 мин
    int targetBreakIntervals = 4;     ///< Целевое число перерывов 50/10 при работе за экраном
    
    // Весовые коэффициенты формулы дисциплины (в сумме дают 1.0)
    double weightSleep = 0.25;        ///< Вес сна в интегральной формуле (25%)
    double weightMeals = 0.20;        ///< Вес питания (20%)
    double weightWater = 0.20;        ///< Вес водного баланса (20%)
    double weightWalk = 0.20;         ///< Вес физической активности (20%)
    double weightBreaks = 0.15;       ///< Вес пауз отдыха 50/10 (15%)
    
    double successThreshold = 80.0;   ///< Порог успешного закрытия дня для продления стрика (%)
};

} // namespace habitflow
