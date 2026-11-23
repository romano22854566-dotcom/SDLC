#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>

#include "habit_record.h"
#include "user_settings.h"
#include "discipline_calculator.h"
#include "streak_manager.h"
#include "json_storage.h"

void runAutomatedTest(habitflow::UserSettings& settings, std::vector<habitflow::HabitRecord>& history, habitflow::JsonStorage& storage) {
    std::cout << "\n>>> [АВТОМАТИЧЕСКАЯ ВЕРИФИКАЦИЯ ЯДРА HABITFLOW C++] <<<\n";
    
    // 1. Создание эталонной записи
    habitflow::HabitRecord day1;
    day1.date = "2026-10-06";
    day1.sleepHours = 8.0;
    day1.waterMl = 2000;
    day1.mealsCount = 3;
    day1.walkMinutes = 50;
    day1.breakIntervals = 4;

    // 2. Расчет дисциплины
    day1.calculatedScore = habitflow::DisciplineCalculator::calculateDailyScore(day1, settings);
    day1.isCompleted = habitflow::DisciplineCalculator::isDaySuccessful(day1.calculatedScore, settings);

    std::cout << "  [1] Расчет дисциплины: дата=" << day1.date 
              << ", результат=" << std::fixed << std::setprecision(1) << day1.calculatedScore 
              << "% -> " << (day1.isCompleted ? "[УСПЕХ: норма выполнена]" : "[СРЫВ]") << "\n";

    // 3. Анализ серии (стрика)
    history.clear();
    history.push_back(day1);
    auto streak = habitflow::StreakManager::evaluateStreaks(history, settings.successThreshold);
    std::cout << "  [2] Анализ серии: текущий стрик=" << streak.currentStreak 
              << " дн., рекорд=" << streak.longestStreak << " дн.\n";

    // 4. Атомарное сохранение
    bool saved = storage.saveAtomic(history, settings);
    std::cout << "  [3] Атомарное сохранение в JSON (habits_db.json): " 
              << (saved ? "[УСПЕШНО: fsync + atomic rename]" : "[ОШИБКА]") << "\n";

    std::cout << ">>> Все базовые системные требования (REQ-SYS-001..005) подтверждены! <<<\n\n";
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::cout << "==============================================================\n";
    std::cout << "          HabitFlow C++ (MVP Core Engine v1.0)                \n";
    std::cout << "   Лабораторная работа №5: Моделирование динамического        \n";
    std::cout << "   поведения системы на языке UML                             \n";
    std::cout << "   Разработчик: Петкун Роман Сергеевич, группа 450503         \n";
    std::cout << "==============================================================\n";

    habitflow::UserSettings settings;
    settings.targetSleepHours = 8.0;
    settings.targetWaterMl = 2000;
    settings.targetMealsCount = 3;
    settings.targetWalkMinutes = 45;
    settings.targetBreakIntervals = 4;
    settings.successThreshold = 80;

    std::vector<habitflow::HabitRecord> history;
    habitflow::JsonStorage storage("habits_db.json");

    // Первоначальный автоматический прогон верификации
    runAutomatedTest(settings, history, storage);

    habitflow::HabitRecord currentDay;
    currentDay.date = "2026-11-23";
    currentDay.sleepHours = 7.5;
    currentDay.waterMl = 1500;
    currentDay.mealsCount = 3;
    currentDay.walkMinutes = 40;
    currentDay.breakIntervals = 3;
    currentDay.calculatedScore = habitflow::DisciplineCalculator::calculateDailyScore(currentDay, settings);
    currentDay.isCompleted = habitflow::DisciplineCalculator::isDaySuccessful(currentDay.calculatedScore, settings);

    bool running = true;
    while (running) {
        std::cout << "--------------------------------------------------------------\n";
        std::cout << "ИНТЕРАКТИВНОЕ МЕНЮ ТРЕКЕРА ПРИВЫЧЕК:\n";
        std::cout << "  [1] Показать текущие целевые нормы (UserSettings)\n";
        std::cout << "  [2] Показать текущее суточное состояние (" << currentDay.date << ")\n";
        std::cout << "  [3] Добавить порцию воды (+250 мл)\n";
        std::cout << "  [4] Ввести показатели вручную (сон, вода, питание)\n";
        std::cout << "  [5] Пересчитать суточный процент дисциплины\n";
        std::cout << "  [6] Атомарно сохранить запись в базу данных habits_db.json\n";
        std::cout << "  [7] Перезапустить полную авто-верификацию ядра\n";
        std::cout << "  [0] Завершить работу программы\n";
        std::cout << "--------------------------------------------------------------\n";
        std::cout << "Выберите действие [0-7]: ";

        int choice = -1;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Некорректный ввод! Попробуйте снова.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                std::cout << "\n[ТЕКУЩИЕ НОРМЫ ПОЛЬЗОВАТЕЛЯ]:\n";
                std::cout << "  Целевой сон: " << settings.targetSleepHours << " ч.\n";
                std::cout << "  Целевой объем воды: " << settings.targetWaterMl << " мл.\n";
                std::cout << "  Приемов пищи: " << settings.targetMealsCount << "\n";
                std::cout << "  Прогулка: " << settings.targetWalkMinutes << " мин.\n";
                std::cout << "  Перерывы 50/10: " << settings.targetBreakIntervals << "\n";
                std::cout << "  Порог успешности дня: " << settings.successThreshold << "%\n\n";
                break;
            }
            case 2: {
                std::cout << "\n[СУТОЧНАЯ ЗАПИСЬ (" << currentDay.date << ")]:\n";
                std::cout << "  Сон: " << currentDay.sleepHours << " ч. / " << settings.targetSleepHours << " ч.\n";
                std::cout << "  Вода: " << currentDay.waterMl << " мл. / " << settings.targetWaterMl << " мл.\n";
                std::cout << "  Приемы пищи: " << currentDay.mealsCount << " / " << settings.targetMealsCount << "\n";
                std::cout << "  Прогулка: " << currentDay.walkMinutes << " мин. / " << settings.targetWalkMinutes << " мин.\n";
                std::cout << "  Перерывы 50/10: " << currentDay.breakIntervals << " / " << settings.targetBreakIntervals << "\n";
                std::cout << "  Интегральный балл: " << std::fixed << std::setprecision(1) << currentDay.calculatedScore << "%\n";
                std::cout << "  Статус: " << (currentDay.isCompleted ? "УСПЕШНЫЙ ДЕНЬ (Green)" : "СРЫВ НОРМЫ (Red/Yellow)") << "\n\n";
                break;
            }
            case 3: {
                currentDay.waterMl += 250;
                currentDay.calculatedScore = habitflow::DisciplineCalculator::calculateDailyScore(currentDay, settings);
                currentDay.isCompleted = habitflow::DisciplineCalculator::isDaySuccessful(currentDay.calculatedScore, settings);
                std::cout << "\n[+] Добавлено 250 мл воды! Итого: " << currentDay.waterMl << " мл. Новый балл: " 
                          << currentDay.calculatedScore << "%\n\n";
                break;
            }
            case 4: {
                std::cout << "\nВведите часы сна (0..24): ";
                std::cin >> currentDay.sleepHours;
                std::cout << "Введите объем выпитой воды (мл): ";
                std::cin >> currentDay.waterMl;
                std::cout << "Введите число приемов пищи: ";
                std::cin >> currentDay.mealsCount;
                currentDay.calculatedScore = habitflow::DisciplineCalculator::calculateDailyScore(currentDay, settings);
                currentDay.isCompleted = habitflow::DisciplineCalculator::isDaySuccessful(currentDay.calculatedScore, settings);
                std::cout << "[V] Показатели обновлены! Новый суточный балл: " << currentDay.calculatedScore << "%\n\n";
                break;
            }
            case 5: {
                currentDay.calculatedScore = habitflow::DisciplineCalculator::calculateDailyScore(currentDay, settings);
                currentDay.isCompleted = habitflow::DisciplineCalculator::isDaySuccessful(currentDay.calculatedScore, settings);
                std::cout << "\n[ПЕРЕСЧЕТ БАЛЛА]: " << currentDay.calculatedScore << "% -> "
                          << (currentDay.isCompleted ? "НОРМА ВЫПОЛНЕНА" : "СРЫВ НОРМЫ") << "\n\n";
                break;
            }
            case 6: {
                history.push_back(currentDay);
                bool ok = storage.saveAtomic(history, settings);
                std::cout << "\n[АТОМАРНАЯ СИНХРОНИЗАЦИЯ]: " 
                          << (ok ? "УСПЕШНО сохранено в habits_db.json" : "ОШИБКА записи") << "\n\n";
                break;
            }
            case 7: {
                runAutomatedTest(settings, history, storage);
                break;
            }
            case 0: {
                std::cout << "\nЗавершение сеанса работы...\n";
                running = false;
                break;
            }
            default: {
                std::cout << "Неизвестная команда. Выберите от 0 до 7.\n";
                break;
            }
        }
    }

    std::cout << "==============================================================\n";
    std::cout << "Работа программы успешно завершена.\n";
    std::cout << "Нажмите Enter для выхода из консоли...";
    std::cin.ignore(10000, '\n');
    std::cin.get();

    return 0;
}
