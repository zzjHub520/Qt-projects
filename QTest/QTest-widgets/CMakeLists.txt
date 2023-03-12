cmake_minimum_required(VERSION 3.5)

project(QTest-widgets LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

find_package(Qt5 COMPONENTS Widgets Test REQUIRED)


add_executable(QTest-widgets
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    testdata.cpp
    testdata.h
)
add_test(NAME QTest-widgets COMMAND QTest-widgets)

target_link_libraries(QTest-widgets PRIVATE Qt5::Widgets Qt5::Test)
