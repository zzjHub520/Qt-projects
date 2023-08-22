# Qt Remote Objects





## Remote Object Concepts

Qt Remote Objects (QtRO) is an Inter-Process Communication (IPC) module developed for Qt. This module extends Qt's existing functionalities to enable information exchange between processes or computers, easily.

One of Qt's key features, to enable this information exchange, is the distinction between an object's API (defined by its properties, signals, and slots) and the implementation of that API. QtRO's purpose is to meet that expected API, even if the true [QObject](https://doc.qt.io/qt-5/qobject.html) is in a different process. A slot called on a copy of an object (the [Replica](https://doc.qt.io/qt-5/qtremoteobjects-replica.html#replica) in QtRO) is forwarded to the true object (the [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) in QtRO) for handling. Every Replica receives updates to the Source, either property changes or emitted signals.

A [Replica](https://doc.qt.io/qt-5/qtremoteobjects-replica.html#replica) is a light-weight proxy for the [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) object, but a Replica supports the same connections and behavior of QObjects, which makes it usable in the same way as any other [QObject](https://doc.qt.io/qt-5/qobject.html) that Qt provides. Behind the scenes, QtRO handles everything that's necessary for the Replica to look like its Source.

Note that Remote Objects behave differently from traditional Remote Procedure Call (RPC) implementations, for example:

- In RPC, the client makes a request and waits for the response.
- In RPC, the server doesn't push anything to the client unless it's in response to a request.
- Often, the design of RPC is such that different clients are independent of each other: for instance, two clients can ask a mapping service for directions and get different results.

While it is possible to implement this RPC-style behavior in QtRO, as Sources without properties, and slots that have return values, QtRO hides the fact that the processing is really remote. You let a node give you the Replica instead of creating it yourself, possibly use the status signals ([isReplicaValid()](https://doc.qt.io/qt-5/qremoteobjectreplica.html#isReplicaValid)), but then interact with the object like you would with any other [QObject](https://doc.qt.io/qt-5/qobject.html)-based type.



## Use Case: GPS

Consider a sensor such as a Global Positioning System (GPS) receiver. In QtRO terms:

- The [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) would be the process that directly interacts with the GPS hardware and derives your current location.
- The location would be exposed as [QObject](https://doc.qt.io/qt-5/qobject.html) properties; the periodic updates to the location would update these properties and emit property changed signals.
- [Replicas](https://doc.qt.io/qt-5/qtremoteobjects-replica.html#replica) would be created in other processes and would always know your current location, but wouldn't need any of the logic to compute the location from the sensor data.
- Connecting to the location changed signal on the Replica would work as expected: the signal emitted from the Source would trigger the signal emission on every Replica.



## Use Case: Printer Access

Consider a service that provides access to a printer. In QtRO terms:

- The [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) would be the process controlling the printer directly.
- The ink levels and printer status would be monitored by [QObject](https://doc.qt.io/qt-5/qobject.html) properties. Updates to these properties would emit property changed signals.
- The key feature -- being able to print something -- needs to be passed back to the printer. Incidentally, this aligns with the Qt slot mechanism, which QtRO uses as the way for [Replicas](https://doc.qt.io/qt-5/qtremoteobjects-replica.html#replica) to make calls on the Source. In effect, properties and signals go from Source to Replicas; slots go from Replica to Source.
- When a print request is accepted, the printer status would change, triggering a change in the status property. This would then be reported to all Replicas.



## Guides

- [Getting Started with Qt Remote Objects](https://doc.qt.io/qt-5/qtremoteobjects-gettingstarted.html)
- [Nodes](https://doc.qt.io/qt-5/qtremoteobjects-node.html)
- [Sources](https://doc.qt.io/qt-5/qtremoteobjects-source.html)
- [Replicas](https://doc.qt.io/qt-5/qtremoteobjects-replica.html)
- [Registry](https://doc.qt.io/qt-5/qtremoteobjects-registry.html)
- [External QIODevices](https://doc.qt.io/qt-5/qtremoteobjects-external-schemas.html)
- [Compiler](https://doc.qt.io/qt-5/qtremoteobjects-repc.html)
- [Remote Object Interaction](https://doc.qt.io/qt-5/qtremoteobjects-interaction.html#remote-object-interaction)
- [Troubleshooting](https://doc.qt.io/qt-5/qtremoteobjects-troubleshooting.html)
- [Protocol Versioning](https://doc.qt.io/qt-5/qtremoteobjects-compatibility.html)



### Reference

- [Qt Remote Objects C++ Classes](https://doc.qt.io/qt-5/qtremoteobjects-module.html)
- [Qt Remote Objects QML Types](https://doc.qt.io/qt-5/qtremoteobjects-qmlmodule.html)



## Licenses

Qt Remote Objects is available under commercial licenses from [The Qt Company](http://www.qt.io/about-us/). In addition, it is available under the [GNU Lesser General Public License, version 3](http://www.gnu.org/licenses/lgpl-3.0.html), or the [GNU General Public License, version 2](http://www.gnu.org/licenses/gpl-2.0.html). See [Qt Licensing](https://doc.qt.io/qt-5/licensing.html) for further details.



© 2023 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.