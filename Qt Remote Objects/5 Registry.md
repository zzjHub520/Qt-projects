# Qt Remote Objects Registry





The registry provides a simpler way to establish these connections. Every node that wants to be part of the registry's network connects to the registry. The registry is itself a specialized source object, and thus is hosted by a node. Connecting to the registry is simply a matter of passing the registry's URL to the [QRemoteObjectNode](https://doc.qt.io/qt-5/qremoteobjectnode.html) or [QRemoteObjectHost](https://doc.qt.io/qt-5/qremoteobjecthost.html) constructor, or passing the URL to the setRegistryUrl method.

The registry is tightly integrated with QtRO. Whenever a [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) is added or removed, the name/URL is updated in the registry automatically. So once your node is connected to the registry, it is not necessary to connect to any other nodes manually. If you request an object on the network and you aren't connected to the hosting node, the registry will know what URL to connect to and will initiate the connection. Once connected (and the list of available objects is passed along, including the desired [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source)), the initialization process for the requested [Replica](https://doc.qt.io/qt-5/qtremoteobjects-replica.html#replica) will start automatically.



© 2023 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.