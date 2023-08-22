# Qt Remote Objects - External QIODevices





## External QIODevices

Qt Remote Objects supports several communications channels out-of-the-box, such as the [QTcpServer](https://doc.qt.io/qt-5/qtcpserver.html) and [QTcpSocket](https://doc.qt.io/qt-5/qtcpsocket.html) pair. Given the desired [QUrl](https://doc.qt.io/qt-5/qurl.html) for tcp, or the desired name (for the [QLocalServer](https://doc.qt.io/qt-5/qlocalserver.html) and [QLocalSocket](https://doc.qt.io/qt-5/qlocalsocket.html) pair), the code needed to listen and connect are boilerplate and handled internally by Qt. Qt Remote Objects supports other types of [QIODevice](https://doc.qt.io/qt-5/qiodevice.html) as well, and the [QRemoteObjectNode](https://doc.qt.io/qt-5/qremoteobjectnode.html) classes provide additional methods to support cases where custom code is needed.

A contrived example with TCP/IP is shown below. A more realistic example would use an SSL connection, which would require configuration of certificates, etc.

```
// Create the server and listen outside of QtRO
QTcpServer tcpServer;
tcpServer.listen(QHostAddress(QStringLiteral("127.0.0.1")), 65213);

// Create the host node.  We don't need a hostUrl unless we want to take
// advantage of external schemas (see next example).
QRemoteObjectHost srcNode;

// Make sure any connections are handed to QtRO
QObject::connect(&tcpServer, &QTcpServer::newConnection, &srcNode,
                 [&srcNode, &tcpServer]() {
    srcNode.addHostSideConnection(tcpServer.nextPendingConnection());
});
```

The Replica side code needs to manually connect to the Host

```
QRemoteObjectNode repNode;
QTcpSocket *socket = new QTcpSocket(&repNode);
QObject::connect(socket, &QTcpSocket::connected, &repNode,
        [socket, &repNode]() {
    repNode.addClientSideConnection(socket);
});
socket->connectToHost(QHostAddress(QStringLiteral("127.0.0.1")), 65213);
```



## External Schemas

It is possible to create each side of the [QIODevice](https://doc.qt.io/qt-5/qiodevice.html) and call [QRemoteObjectNode::addClientSideConnection](https://doc.qt.io/qt-5/qremoteobjectnode.html#addClientSideConnection)(QIODevice *ioDevice) and [QRemoteObjectHostBase::addHostSideConnection](https://doc.qt.io/qt-5/qremoteobjecthostbase.html#addHostSideConnection)(QIODevice *ioDevice) as shown above. This is fully supported, but requires the client know how to establish the connection or have a way to discover that information. This is exactly the problem the registry was designed to solve.

Qt Remote Objects also allows "External Schemas" to be used with the registry, which helps with connection setup. On the [QRemoteObjectHost](https://doc.qt.io/qt-5/qremoteobjecthost.html) side, the user must set the hostUrl with the desired schema.

```
// Use standard tcp url for the registry
const QUrl registryUrl = QUrl(QStringLiteral("tcp://127.0.0.1:65212"));
// Use "exttcp" for the "external" interface
const QUrl extUrl = QUrl(QStringLiteral("exttcp://127.0.0.1:65213"));

// Create the server and listen outside of QtRO
QTcpServer tcpServer;
tcpServer.listen(QHostAddress(extUrl.host()), extUrl.port());

// We need a registry for everyone to connect to
QRemoteObjectRegistryHost registry(registryUrl);

// Finally, we create our host node and register "exttcp" as our schema.
// We need the AllowExternalRegistration parameter to prevent the node from
// setting a hostUrlInvalid error.
QRemoteObjectHost srcNode(extUrl, registryUrl, QRemoteObjectHost::AllowExternalRegistration);
// From now on, when we call enableRemoting() from this node, the registry
// will be updated to show the Source object at extUrl.
```

On the [Replica](https://doc.qt.io/qt-5/qtremoteobjects-replica.html#replica) side, the [QRemoteObjectNode](https://doc.qt.io/qt-5/qremoteobjectnode.html) needs to register a callback to be used when the external schema is detected. The callback must be a [RemoteObjectSchemaHandler](https://doc.qt.io/qt-5/qremoteobjectnode.html#RemoteObjectSchemaHandler-typedef).

```
// Use standard tcp url for the registry
const QUrl registryUrl = QUrl(QStringLiteral("tcp://127.0.0.1:65212"));

// This time create the node connected to the registry
QRemoteObjectNode repNode(registryUrl);

// Create the RemoteObjectSchemaHandler callback
QRemoteObjectNode::RemoteObjectSchemaHandler setupTcp = [&repNode](QUrl url) {
    QTcpSocket *socket = new QTcpSocket(&repNode);
    connect(socket, &QTcpSocket::connected,
            [socket, &repNode]() {
        repNode.addClientSideConnection(socket);
    });
    connect(socket, &QSslSocket::errorOccurred,
            [socket](QAbstractSocket::SocketError error) {
        delete socket;
    });
    socket->connectToHost(url.host(), url.port());
};

// Once we call registerExternalSchema, the above method will be called
// whenever the registry sees an object we are interested in on "exttcp"
repNode.registerExternalSchema(QStringLiteral("exttcp"), setupTcp);
```



© 2023 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.