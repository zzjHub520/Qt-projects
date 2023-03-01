cmake_minimum_required(VERSION 3.5)

project(PostgresSQL LANGUAGES CXX)

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

find_package(Qt5 COMPONENTS Widgets Sql REQUIRED)

file(COPY
    bin/libcrypto-3-x64.dll
    bin/libiconv-2.dll
    bin/libintl-9.dll
    bin/libpq.dll
    bin/libssl-3-x64.dll
    bin/libwinpthread-1.dll
    DESTINATION ${PROJECT_BINARY_DIR})

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

if(ANDROID)
  add_library(PostgresSQL SHARED
    ${PROJECT_SOURCES}
  )
else()
  add_executable(PostgresSQL
    ${PROJECT_SOURCES}
  )
endif()

target_link_libraries(PostgresSQL PRIVATE Qt5::Widgets Qt5::Sql)
