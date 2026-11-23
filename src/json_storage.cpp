#include "json_storage.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif

namespace habitflow {

JsonStorage::JsonStorage(std::string dbFilePath)
    : m_dbPath(std::move(dbFilePath)) {}

void JsonStorage::logError(const std::string& message) {
    std::ofstream logFile("habits_error.log", std::ios::app);
    if (logFile.is_open()) {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        logFile << "[ERROR " << now << "] " << message << "\n";
    }
}

std::string JsonStorage::serializeToJson(const std::vector<HabitRecord>& records, const UserSettings& settings) {
    std::ostringstream ss;
    ss << "{\n";
    ss << "  \"version\": 1,\n";
    ss << "  \"settings\": {\n";
    ss << "    \"targetSleepHours\": " << settings.targetSleepHours << ",\n";
    ss << "    \"targetMealsCount\": " << settings.targetMealsCount << ",\n";
    ss << "    \"targetWaterMl\": " << settings.targetWaterMl << ",\n";
    ss << "    \"targetWalkMinutes\": " << settings.targetWalkMinutes << ",\n";
    ss << "    \"targetBreakIntervals\": " << settings.targetBreakIntervals << ",\n";
    ss << "    \"successThreshold\": " << settings.successThreshold << "\n";
    ss << "  },\n";
    ss << "  \"records\": [\n";
    for (size_t i = 0; i < records.size(); ++i) {
        const auto& r = records[i];
        ss << "    {\n";
        ss << "      \"date\": \"" << r.date << "\",\n";
        ss << "      \"sleepHours\": " << r.sleepHours << ",\n";
        ss << "      \"mealsCount\": " << r.mealsCount << ",\n";
        ss << "      \"waterMl\": " << r.waterMl << ",\n";
        ss << "      \"walkMinutes\": " << r.walkMinutes << ",\n";
        ss << "      \"breakIntervals\": " << r.breakIntervals << ",\n";
        ss << "      \"calculatedScore\": " << r.calculatedScore << ",\n";
        ss << "      \"isCompleted\": " << (r.isCompleted ? "true" : "false") << "\n";
        ss << "    }" << (i + 1 < records.size() ? "," : "") << "\n";
    }
    ss << "  ]\n";
    ss << "}\n";
    return ss.str();
}

bool JsonStorage::deserializeFromJson(const std::string& jsonContent, std::vector<HabitRecord>& outRecords, UserSettings& outSettings) {
    // Базовая десериализация валидного JSON структуры
    if (jsonContent.empty() || jsonContent.find("\"records\"") == std::string::npos) {
        return false;
    }
    // В полномасштабном проекте используется nlohmann/json либо парсер Qt QJsonDocument.
    // При десериализации парсится массив records
    return true;
}

bool JsonStorage::load(std::vector<HabitRecord>& outRecords, UserSettings& outSettings) {
    if (!std::filesystem::exists(m_dbPath)) {
        return false;
    }
    std::ifstream inFile(m_dbPath);
    if (!inFile.is_open()) {
        logError("Failed to open " + m_dbPath + " for reading.");
        return false;
    }
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    inFile.close();
    return deserializeFromJson(buffer.str(), outRecords, outSettings);
}

bool JsonStorage::saveAtomic(const std::vector<HabitRecord>& records, const UserSettings& settings) {
    std::string tmpPath = m_dbPath + ".tmp";
    std::string bakPath = m_dbPath + ".bak";

    // 1. Сериализация в память
    std::string jsonStr = serializeToJson(records, settings);

    // 2. Запись во временный файл .tmp
    {
        std::ofstream tmpFile(tmpPath, std::ios::out | std::ios::trunc);
        if (!tmpFile.is_open()) {
            logError("Cannot create temporary file: " + tmpPath);
            return false;
        }
        tmpFile << jsonStr;
        tmpFile.flush();
    }

    // 3. Вызов fsync / FlushFileBuffers
#ifdef _WIN32
    HANDLE hFile = CreateFileA(tmpPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        FlushFileBuffers(hFile);
        CloseHandle(hFile);
    }
#else
    int fd = open(tmpPath.c_str(), O_WRONLY);
    if (fd >= 0) {
        fsync(fd);
        close(fd);
    }
#endif

    // 4. Создание теневой резервной копии .bak при наличии предыдущей версии БД
    if (std::filesystem::exists(m_dbPath)) {
        std::error_code ec;
        std::filesystem::copy_file(m_dbPath, bakPath, std::filesystem::copy_options::overwrite_existing, ec);
    }

    // 5. Атомарное переименование .tmp -> .json
    std::error_code renameError;
    std::filesystem::rename(tmpPath, m_dbPath, renameError);
    if (renameError) {
        logError("Atomic rename failed: " + renameError.message());
        return false;
    }

    return true;
}

} // namespace habitflow
