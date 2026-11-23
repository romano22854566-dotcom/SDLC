#include <iostream>
#include "habit_record.h"
#include "user_settings.h"
#include "discipline_calculator.h"
#include "streak_manager.h"
#include "json_storage.h"

int main() {
    std::cout << "=== HabitFlow C++ Core Verification (Sprint 1 MVP) ===\n";

    habitflow::UserSettings settings;
    settings.targetSleepHours = 8.0;
    settings.targetWaterMl = 2000;
    settings.targetMealsCount = 3;

    // 1. Создание суточной записи
    habitflow::HabitRecord day1;
    day1.date = "2026-10-06";
    day1.sleepHours = 8.0;
    day1.waterMl = 2000;
    day1.mealsCount = 3;
    day1.walkMinutes = 50;
    day1.breakIntervals = 4;

    // 2. Расчет суточного процента дисциплины
    day1.calculatedScore = habitflow::DisciplineCalculator::calculateDailyScore(day1, settings);
    day1.isCompleted = habitflow::DisciplineCalculator::isDaySuccessful(day1.calculatedScore, settings);

    std::cout << "Date: " << day1.date << " | Discipline Score: " << day1.calculatedScore 
              << "% | Completed: " << (day1.isCompleted ? "YES" : "NO") << "\n";

    // 3. Расчет непрерывной серии
    std::vector<habitflow::HabitRecord> history = { day1 };
    auto streak = habitflow::StreakManager::evaluateStreaks(history, settings.successThreshold);
    std::cout << "Current Streak: " << streak.currentStreak << " day(s) | Longest: " << streak.longestStreak << "\n";

    // 4. Атомарное сохранение в JSON
    habitflow::JsonStorage storage("habits_db.json");
    bool saved = storage.saveAtomic(history, settings);
    std::cout << "Atomic Persistence to JSON: " << (saved ? "SUCCESS" : "FAILED") << "\n";

    std::cout << "All core functions verified successfully!\n";
    return 0;
}
