cmake_minimum_required(VERSION 3.20)
project(carService LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Top-level executable (kept for compatibility)

# Task 1
add_executable(task1 task1/testQueue.cpp)
target_include_directories(task1 PRIVATE ${CMAKE_SOURCE_DIR}/task1)

# Task 2
add_executable(task2 task2/testTask2.cpp)
target_include_directories(task2 PRIVATE ${CMAKE_SOURCE_DIR}/task2)

# Task 3
add_executable(task3 task3/testCarStation.cpp)
target_include_directories(task3 PRIVATE ${CMAKE_SOURCE_DIR}/task3)

# Task 4
add_executable(task4 task4/testSemaphore.cpp)
target_include_directories(task4 PRIVATE ${CMAKE_SOURCE_DIR}/task4)

# Task 5 (uses bundled nlohmann single-header)
add_executable(task5 task5/main.cpp)
target_include_directories(task5 PRIVATE ${CMAKE_SOURCE_DIR}/task5)

# Generator executable (required by task5 at runtime)
add_executable(generator task5/generator.cpp)
target_include_directories(generator PRIVATE ${CMAKE_SOURCE_DIR}/task5)

# Ensure task5 and generator are placed in the same runtime directory
## Place runtime binaries in config-specific output directories so both
## `task5` and `generator` end up together for multi-config generators
set_target_properties(task5 PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>"
)
set_target_properties(generator PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>"
)

# Ensure building task5 will also build generator so it's available at runtime
add_dependencies(task5 generator)

# Convenience: group all task targets under a custom target
add_custom_target(all_tasks DEPENDS task1 task2 task3 task4 task5 generator)
## (removed accidental duplicate `Test` target)
