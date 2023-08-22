# qt5_generate_repc





## Synopsis

```cmake
qt5_generate_repc(<VAR> rep_file output_type)
```



## Description

Creates rules for calling [repc](https://doc.qt.io/qt-5/qtremoteobjects-repc.html) on `rep_file`. `output_type` must be either `SOURCE` or `REPLICA`. The paths of the generated files are added to `<VAR>`.

Depending on the `output_type` argument, the generated code will either implement a [Qt Remote Objects Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html) or a [Qt Remote Objects Replica](https://doc.qt.io/qt-5/qtremoteobjects-replica.html) type in C++.



## Examples

```cmake
find_package(Qt5 COMPONENTS RemoteObjects REQUIRED)

set(SOURCES
    main.cpp
    simpleswitch.cpp
)

qt5_generate_repc(SOURCES simpleswitch.rep SOURCE)

add_executable(directconnectserver ${SOURCES})
target_link_libraries(directconnectserver Qt5::RemoteObjects)
```



© 2023 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.