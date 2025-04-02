# Корневой CMakeLists.txt
cmake_minimum_required(VERSION 3.5)
project(mediaPlayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_POLICY CMP0135 NEW)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE ${CMAKE_SOURCE_DIR}/include)
set(SRC ${CMAKE_SOURCE_DIR}/src)

# Qt настройки
set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.12.12\\5.12.12\\msvc2017_64")
find_package(Qt5 COMPONENTS Widgets Multimedia MultimediaWidgets Bluetooth REQUIRED)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

add_subdirectory(src)