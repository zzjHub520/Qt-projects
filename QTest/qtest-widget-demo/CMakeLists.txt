cmake_minimum_required(VERSION 3.5)

project(qtest-widget-demo LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

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


