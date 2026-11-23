#pragma once

#include "habit_record.h"
#include "user_settings.h"
#include <string>
#include <vector>

namespace habitflow {

/**
 * @brief Интерфейс постоянного хранилища данных HabitFlow
 */
class IHabitStorage {
public:
    virtual ~IHabitStorage() = default;
    virtual bool load(std::vector<HabitRecord>& outRecords, UserSettings& outSettings) = 0;
    virtual bool saveAtomic(const std::vector<HabitRecord>& records, const UserSettings& settings) = 0;
};

/**
 * @brief Реализация локального атомарного хранилища на базе JSON (ADR-0002)
 */
class JsonStorage : public IHabitStorage {
public:
    explicit JsonStorage(std::string dbFilePath = "habits_db.json");

    bool load(std::vector<HabitRecord>& outRecords, UserSettings& outSettings) override;
    bool saveAtomic(const std::vector<HabitRecord>& records, const UserSettings& settings) override;

    const std::string& getFilePath() const { return m_dbPath; }

private:
    std::string m_dbPath;
    std::string serializeToJson(const std::vector<HabitRecord>& records, const UserSettings& settings);
    bool deserializeFromJson(const std::string& jsonContent, std::vector<HabitRecord>& outRecords, UserSettings& outSettings);
    void logError(const std::string& message);
};

} // namespace habitflow
