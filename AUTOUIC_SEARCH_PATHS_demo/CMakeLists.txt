cmake_minimum_required(VERSION 3.22)

project(untitled LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "form")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(untitled
        main.cpp
        mainwindow.cpp
        mainwindow.h
        form/mainwindow.ui
        )
target_link_libraries(untitled PRIVATE Qt5::Widgets)


