cmake_minimum_required(VERSION 3.20)
project(HabitFlow LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Каталоги заголовочных файлов
include_directories(include)

# Исходные файлы библиотеки ядра
set(CORE_SOURCES
    src/discipline_calculator.cpp
    src/streak_manager.cpp
    src/json_storage.cpp
)

set(CORE_HEADERS
    include/habit_record.h
    include/user_settings.h
    include/discipline_calculator.h
    include/streak_manager.h
    include/json_storage.h
)

# Статическая библиотека ядра приложения
add_library(habitflow_core STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# Демонстрационный исполняемый модуль проверки функционала Sprint 1
add_executable(habitflow_core_demo src/main_demo.cpp)
target_link_libraries(habitflow_core_demo PRIVATE habitflow_core)
