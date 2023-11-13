cmake_minimum_required(VERSION 3.5)

project(qtest-widget-demo LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAke_code)

find_package(Qt5 COMPONENTS Widgets Test REQUIRED)
enable_testing()

add_executable(qtest-widget-demo
    main.cpp
    widget.cpp
    widget.h
    widget.ui
    Attach.cpp
    Attach.h
    )
target_link_libraries(qtest-widget-demo PRIVATE Qt5::Widgets)

add_executable(qtest-widget-demo-test QTestDemo.cpp widget.cpp Attach.cpp)
add_test(NAME qtest-widget-demo-test COMMAND qtest-widget-demo-test)
target_link_libraries(qtest-widget-demo-test PRIVATE Qt5::Widgets Qt5::Test)


