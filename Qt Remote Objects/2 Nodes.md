# Qt Remote Objects Nodes





In a QtRO network, information is passed between processes via QRemoteObjectNodes ("nodes"). This peer-to-peer functionality uses a small number of distinct packets passing the necessary data between nodes.

Each process that participates in the network instantiates a Node-based type, such as [QRemoteObjectNode](https://doc.qt.io/qt-5/qremoteobjectnode.html), [QRemoteObjectHost](https://doc.qt.io/qt-5/qremoteobjecthost.html), or [QRemoteObjectRegistryHost](https://doc.qt.io/qt-5/qremoteobjectregistryhost.html). The host types of Nodes provide additional functionality. Both [QRemoteObjectHost](https://doc.qt.io/qt-5/qremoteobjecthost.html) and [QRemoteObjectRegistryHost](https://doc.qt.io/qt-5/qremoteobjectregistryhost.html) support the [enableRemoting()](https://doc.qt.io/qt-5/qremoteobjecthostbase.html#enableRemoting) and the corresponding [disableRemoting()](https://doc.qt.io/qt-5/qremoteobjecthostbase.html#disableRemoting)) methods, which are the key methods to expose source objects to the network. To use the [Registry](https://doc.qt.io/qt-5/qtremoteobjects-registry.html#registry) functionality, you need to have a [QRemoteObjectRegistryHost](https://doc.qt.io/qt-5/qremoteobjectregistryhost.html) on the network. Then, all other nodes can pass the RegistryHost's URL to the Node's `registryAddress` constructor parameter, or pass the URL to the [setRegistryUrl()](https://doc.qt.io/qt-5/qremoteobjectnode.html#registryUrl-prop) method.

Since QtRO is a peer-to-peer network, to [acquire()](https://doc.qt.io/qt-5/qremoteobjectnode.html#acquire) a valid [Replica](https://doc.qt.io/qt-5/qtremoteobjects-replica.html#replica), the replica's node needs to be connected to the node that hosts its [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source). A host node is a node that allows other nodes to connect to it, which is accomplished by giving hosts unique addresses. This address is provided to the [QRemoteObjectHost](https://doc.qt.io/qt-5/qremoteobjecthost.html) constructor or set by the setHostUrl method. The node from which a replica is requested must establish the connection to the host node, to initialize the replica and keep it up to date.



## Connecting Nodes using QtRO URLs

Host Nodes use custom URLs to simplify connections. Currently, QtRO supports two types of connections:

1. A TCP connection using the standard TCP/IP protocol - supports connections between devices as well as between processes on the same device.
2. A local connection - supports connections between processes on the same device. This type of connection can have less overhead, depending on the underlying Operating System features.

For local connections, you must use a unique name. For TCP connections, you must provide a unique address and port number combination.

Currently, QtRO does not include a [zeroconf](http://www.zeroconf.org/) facility. Therefore, all processes or devices must know beforehand, how to connect to each other. A [QRemoteObjectRegistry](https://doc.qt.io/qt-5/qremoteobjectregistry.html) can be used to simplify the connection process for a network with multiple Host Nodes.

The table below summarizes the connection types available:

|                             URL                              |                          Host Node                           |                       Connecting Node                        |
| :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
|  [QUrl](https://doc.qt.io/qt-5/qurl.html)("local:service")   | [QLocalServer](https://doc.qt.io/qt-5/qlocalserver.html)("service") | [QLocalSocket](https://doc.qt.io/qt-5/qlocalsocket.html)("service") |
| [QUrl](https://doc.qt.io/qt-5/qurl.html)("tcp://192.168.1.1:9999") | [QTcpServer](https://doc.qt.io/qt-5/qtcpserver.html)("192.168.1.1",9999) | [QTcpSocket](https://doc.qt.io/qt-5/qtcpsocket.html)("192.168.1.1",9999) |

Nodes have a few [enableRemoting()](https://doc.qt.io/qt-5/qremoteobjecthostbase.html#enableRemoting) methods that are used to share objects on the network. However, if the node is not a host node, an error is returned.

Other processes or devices that want to interact with a shared object can use one of the [node's acquire()](https://doc.qt.io/qt-5/qremoteobjectnode.html#acquire) methods, to instantiate a replica.



© 2023 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.