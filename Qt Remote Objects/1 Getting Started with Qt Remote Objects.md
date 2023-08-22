# Getting Started with Qt Remote Objects





## Introduction

The Qt Remote Objects module provides an easy way to share Qt APIs between processes and devices. For this to work, we require a data channel between processes and devices. To establish this data channel, first, you need a [QRemoteObjectNode](https://doc.qt.io/qt-5/qremoteobjectnode.html).

In QtRO, a node is an endpoint for communication. In a remote objects network, each participant, be it a process or a device, needs its own node. QtRO is a peer-to-peer network, with connected nodes being the links in the network.

Nodes, by themselves, don’t provide much use. But their value comes when you add [QObject](https://doc.qt.io/qt-5/qobject.html) classes to a node to share. Then, any peer node can request a copy or instance of the shared object from the *host node*, the node that shares it.

Unlike when using normal class instances (with independent properties and signal emissions), QtRO automatically synchronizes changes to the shared object across all of its copies. With a few exceptions, these copies have the identical Qt API as the original object, and are meant to be used exactly as if the original object were available.

In QtRO, the original object is called the [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source). It's a fully-implemented C++ class, with the necessary business logic to provide the required functionality. Copies of this object are called [Replica](https://doc.qt.io/qt-5/qtremoteobjects-replica.html#replica)s. You don’t need to write any C++ code for a replica; you request an instance from a node instead. While you do need some code to use the replica, such as connecting signals to your own slots, you don’t need to implement the internal behavior – that's already done for you in the source.

Because the source can be in a different process or even on a different device, there are factors in QtRO that you need to consider, which you typically wouldn't run into when developing without Inter-Process Communication (IPC). Specifically, what happens if the process or device isn't there? This is where the additions to the Replica API come in:

- The [initialized()](https://doc.qt.io/qt-5/qremoteobjectreplica.html#initialized) signal is emitted once the replica has received the [source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) state from the QtRO network.
- Both the [isReplicaValid](https://doc.qt.io/qt-5/qremoteobjectreplica.html#isReplicaValid) property and the [stateChanged()](https://doc.qt.io/qt-5/qremoteobjectreplica.html#stateChanged) signal alert you if the connection is lost.

Objects shared over QtRO use the links (conduits) between nodes for all communication. If you want to share a [QObject](https://doc.qt.io/qt-5/qobject.html), you must create a *host node* with a URL other nodes can connect to. You can also use the [Registry](https://doc.qt.io/qt-5/qtremoteobjects-registry.html#registry) to facilitate connections, but your nodes that share [sources](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) still need to be host nodes. Each shared object is given a name (a [QString](https://doc.qt.io/qt-5/qstring.html)), used to identify it on the QtRO network.



## Implementation

To illustrate the use of remote objects, on the source side, we need to:

1. Create the [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) object that is replicated to other nodes, with or without using [repc](https://doc.qt.io/qt-5/qtremoteobjects-repc.html), the Qt Remote Objects Compiler.
2. Optionally, create the [Registry](https://doc.qt.io/qt-5/qtremoteobjects-registry.html#registry). Otherwise, use direct connections.
3. Create a host node so that the source object can be shared.
4. Call the node's [enableRemoting()](https://doc.qt.io/qt-5/qremoteobjecthostbase.html#enableRemoting) function to share the source object.

On the replica side, we need to:

1. Optionally, use [repc](https://doc.qt.io/qt-5/qtremoteobjects-repc.html) to generate a [Replica](https://doc.qt.io/qt-5/qtremoteobjects-replica.html#replica) header for your project.
2. Create the node that will connect with the [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) host node.
3. Call the node's [acquire()](https://doc.qt.io/qt-5/qremoteobjectnode.html#acquire) function to create a pointer to a replica.

The following examples illustrate both [repc](https://doc.qt.io/qt-5/qtremoteobjects-repc.html)-compiled static objects and dynamic source objects. Additionally, they also show direct connections as well as connections that use a [Registry](https://doc.qt.io/qt-5/qtremoteobjects-registry.html#registry) between nodes.

- [Example 1: Direct Connection using a Static Source](https://doc.qt.io/qt-5/remoteobjects-example-static-source.html)
- [Example 2: Direct Connection with a Dynamic Replica](https://doc.qt.io/qt-5/remoteobjects-example-dynamic-replica.html)
- [Example 3: Connections to Remote Nodes using a Registry](https://doc.qt.io/qt-5/remoteobjects-example-registry.html)



© 2023 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.