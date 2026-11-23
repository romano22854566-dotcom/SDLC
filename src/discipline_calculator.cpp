#include "discipline_calculator.h"
#include <algorithm>

namespace habitflow {

double DisciplineCalculator::calculateDailyScore(const HabitRecord& record, const UserSettings& settings) {
    // 1. Компонента сна (бинарная шкала или отношение к норме)
    double sleepRatio = (record.sleepHours >= settings.targetSleepHours) ? 1.0 : (record.sleepHours / std::max(1.0, settings.targetSleepHours));
    sleepRatio = std::clamp(sleepRatio, 0.0, 1.0);

    // 2. Компонента питания (отношение фактически выполненных приемов пищи к целевому)
    double mealRatio = static_cast<double>(record.mealsCount) / std::max(1, settings.targetMealsCount);
    mealRatio = std::clamp(mealRatio, 0.0, 1.0);

    // 3. Компонента воды (отношение выпитых мл к суточной норме)
    double waterRatio = static_cast<double>(record.waterMl) / std::max(1, settings.targetWaterMl);
    waterRatio = std::clamp(waterRatio, 0.0, 1.0);

    // 4. Компонента прогулки
    double walkRatio = static_cast<double>(record.walkMinutes) / std::max(1, settings.targetWalkMinutes);
    walkRatio = std::clamp(walkRatio, 0.0, 1.0);

    // 5. Компонента 10-минутных пауз отдыха по таймеру 50/10
    double breakRatio = static_cast<double>(record.breakIntervals) / std::max(1, settings.targetBreakIntervals);
    breakRatio = std::clamp(breakRatio, 0.0, 1.0);

    // Взвешенная аддитивная свертка 5 факторов
    double weightedScore = 
        settings.weightSleep * sleepRatio +
        settings.weightMeals * mealRatio +
        settings.weightWater * waterRatio +
        settings.weightWalk * walkRatio +
        settings.weightBreaks * breakRatio;

    return std::clamp(weightedScore * 100.0, 0.0, 100.0);
}

bool DisciplineCalculator::isDaySuccessful(double score, const UserSettings& settings) {
    return score >= settings.successThreshold;
}

} // namespace habitflow
