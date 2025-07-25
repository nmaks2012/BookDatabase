cmake_minimum_required(VERSION 3.30)
project(BookDB VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Все предупреждения - это ошибки
    # add_compile_options(-Wfatal-errors -Wall -Werror)
    add_compile_options(-Wfatal-errors -Wall -Werror -g -O1 -Wextra -pedantic)

    # Не больше одной ошибки за раз
    add_compile_options(-fmax-errors=1)
endif()

# Ищем необходимые библиотеки
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

file(GLOB HEADER_FILES "${CMAKE_SOURCE_DIR}/include/*.hpp")

# Создаём статическую библиотеку
add_library(${PROJECT_NAME}_imp STATIC "${CMAKE_SOURCE_DIR}/src/main.cpp" ${HEADER_FILES})

# Добавляем в проект используемые сторонние библиотеки
target_include_directories(${PROJECT_NAME}_imp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME}_imp PRIVATE ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})

# Создаём исполняемый таргет и линкуем к нему статическую библиотеку
add_executable(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/src/main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_imp)

#
# Тесты
#

# Рекурсивно ищем все используемые в тестах .cpp файлы
file(GLOB TEST_SRC_FILES "${CMAKE_SOURCE_DIR}/tests/*.cpp")

add_executable(${PROJECT_NAME}_tests "${TEST_SRC_FILES}")
target_link_libraries(${PROJECT_NAME}_tests PRIVATE ${PROJECT_NAME}_imp GTest::GTest GTest::Main)
target_include_directories(${PROJECT_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# benchmark
add_executable(${PROJECT_NAME}_benchmark "${CMAKE_SOURCE_DIR}/benchmark/main.cpp")
target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_imp benchmark::benchmark_main)
target_include_directories(${PROJECT_NAME}_benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Включаем тестирование
enable_testing()
add_test(NAME BookDB_Tests COMMAND ${PROJECT_NAME}_tests)
