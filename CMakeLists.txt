cmake_minimum_required(VERSION 3.15)
project(MyQuantProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Api.cpp
    src/DataManager.cpp
    src/MovingAverageCrossoverStrategy.cpp
    src/Backtester.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        CURL::libcurl
        nlohmann_json::nlohmann_json
)

enable_testing()
add_subdirectory(test)
