# Qt Remote Objects C++ Classes



Provides an easy to use mechanism for sharing a [QObject](https://doc.qt.io/qt-5/qobject.html)'s Properties, Signals, or Slots, between processes. [More...](https://doc.qt.io/qt-5/qtremoteobjects-module.html#details)



## Namespaces

| [QtRemoteObjects](https://doc.qt.io/qt-5/qtremoteobjects.html) | Contains identifiers used in the Remote Objects module, as well as some functions used from code generated by the Replica Compiler (repc) |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
|                                                              |                                                              |



## Classes

| [QRemoteObjectAbstractPersistedStore](https://doc.qt.io/qt-5/qremoteobjectabstractpersistedstore.html) | A class which provides the methods for setting PROP values of a replica to value they had the last time the replica was used |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [QRemoteObjectDynamicReplica](https://doc.qt.io/qt-5/qremoteobjectdynamicreplica.html) | A dynamically instantiated Replica                           |
| [QRemoteObjectHost](https://doc.qt.io/qt-5/qremoteobjecthost.html) | A (Host) Node on a Qt Remote Objects network                 |
| [QRemoteObjectHostBase](https://doc.qt.io/qt-5/qremoteobjecthostbase.html) | Base functionality common to Host and RegistryHost classes   |
| [QRemoteObjectNode](https://doc.qt.io/qt-5/qremoteobjectnode.html) | A node on a Qt Remote Objects network                        |
| [QRemoteObjectPendingCall](https://doc.qt.io/qt-5/qremoteobjectpendingcall.html) | Encapsulates the result of an asynchronous method call       |
| [QRemoteObjectPendingCallWatcher](https://doc.qt.io/qt-5/qremoteobjectpendingcallwatcher.html) | Provides a QObject-based API for watching a QRemoteObjectPendingCall |
| [QRemoteObjectPendingReply](https://doc.qt.io/qt-5/qremoteobjectpendingreply.html) | A templated version of QRemoteObjectPendingCall              |
| [QRemoteObjectRegistry](https://doc.qt.io/qt-5/qremoteobjectregistry.html) | A class holding information about Source objects available on the Qt Remote Objects network |
| [QRemoteObjectRegistryHost](https://doc.qt.io/qt-5/qremoteobjectregistryhost.html) | A (Host/Registry) node on a Qt Remote Objects network        |
| [QRemoteObjectReplica](https://doc.qt.io/qt-5/qremoteobjectreplica.html) | A class interacting with (but not implementing) a Qt API on the Remote Object network |
| [QtROClientFactory](https://doc.qt.io/qt-5/qtroclientfactory.html) | A class that holds information about client backends available on the Qt Remote Objects network |
| [QtROServerFactory](https://doc.qt.io/qt-5/qtroserverfactory.html) | A class that holds information about server backends available on the Qt Remote Objects network |



## Detailed Description

To include the definitions of the module's classes, use the following directive:

```cpp
#include <QtRemoteObjects>
```

To link against the module, add this line to your qmake `.pro` file:

```qmake
QT += remoteobjects
```

For more information, see [Getting Started with Qt Remote Objects](https://doc.qt.io/qt-5/qtremoteobjects-gettingstarted.html).



© 2023 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.