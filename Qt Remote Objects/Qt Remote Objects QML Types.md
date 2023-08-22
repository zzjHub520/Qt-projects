# Qt Remote Objects QML Types





The QML types for [Qt Remote Objects](https://doc.qt.io/qt-5/qtremoteobjects-index.html) provide the helper pieces needed to build a remote objects network. They are typically used in conjunction with custom-registered replica types that make up a specific network.

As an example, consider the following .rep file:

```
class MyType {
    PROP(QString myProp="Hello World")
};
```

The generated replica can be registered as a QML type:

```
qmlRegisterType<MyTypeReplica>("custom",1,0,"MyTypeReplica")
```

And then used from QML in conjunction with the base type Node:

```qml
import QtQuick 2.0
import QtRemoteObjects 5.15
import custom 1.0

Item {
    MyTypeReplica {
        id: myType
        node: Node { registryUrl: "local:registry" }
    }

    Text { text: myType.myProp }

    MouseArea {
        anchors.fill: parent
        onClicked: myType.pushMyProp("Updated Text")
    }
}
```

Note that by default you cannot directly assign to a replica property, but rather use a `push` function. This is due to the potential problems that arise from the mix of declarative programming and asynchronous updates. Specifically, we want to avoid issues like the following:

```
myType.myProp = "Updated Text"
console.log(myType.myProp) // logs "Hello World", as the new text has not yet been round-tripped
```

The QML types in this module can be imported into your application using the following import statement in your .qml file:

```
import QtRemoteObjects 5.15
```



## QML Types

| [Host](https://doc.qt.io/qt-5/qml-qtremoteobjects-host.html) | A host node on a Qt Remote Objects network                   |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [Node](https://doc.qt.io/qt-5/qml-qtremoteobjects-node.html) | A node on a Qt Remote Objects network                        |
| [QtRemoteObjects](https://doc.qt.io/qt-5/qml-qtremoteobjects-qtremoteobjects.html) | Global object provides useful functions for working with remote types in QML |
| [SettingsStore](https://doc.qt.io/qt-5/qml-qtremoteobjects-settingsstore.html) | A basic store for persisted properties                       |



© 2023 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.