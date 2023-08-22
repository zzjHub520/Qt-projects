# Qt Remote Objects Source





At a high level, you can choose to use a [QObject](https://doc.qt.io/qt-5/qobject.html) type directly as a source; or you can define the desired API in a `.rep` template for use with the [repc](https://doc.qt.io/qt-5/qtremoteobjects-repc.html) compiler.

If you already have a fully defined [QObject](https://doc.qt.io/qt-5/qobject.html), you can use it as a source by passing it to [QRemoteObjectHostBase::enableRemoting](https://doc.qt.io/qt-5/qremoteobjecthostbase.html#enableRemoting)(). This way, other processes or devices can then create [dynamics replicas](https://doc.qt.io/qt-5/qremoteobjectdynamicreplica.html) of the object to interact with.

For more information, see [Remote Object Interaction](https://doc.qt.io/qt-5/qtremoteobjects-interaction.html#remote-object-interaction)).

Letting repc generate a source header file for your project, using the [REPC_SOURCE](https://doc.qt.io/qt-5/qtremoteobjects-repc.html#repc-source) variable, provides three different options to implement the required API.

Suppose your class name is Foo, you then have the following options:

- [FooSimpleSource](https://doc.qt.io/qt-5/qtremoteobjects-source.html#typesimplesource) inheritance
- [FooSource](https://doc.qt.io/qt-5/qtremoteobjects-source.html#typesource) inheritance
- [FooSourceAPI](https://doc.qt.io/qt-5/qtremoteobjects-source.html#typeapi) usage with your own [QObject](https://doc.qt.io/qt-5/qobject.html)

For more details on how to create a `.rep` file, see [The rep file format](https://doc.qt.io/qt-5/qtremoteobjects-repc.html#the-rep-file-format).



There is a <Type>SimpleSource class defined in the header, which provides the basic getter/setter methods for each property and implements data members of the correct property type in the header. "<Type>" here represents the class name from the `.rep` file, so if your class is of type "MyType" in the `.rep` file, then the generated header file will have a MyTypeSimpleSource class declared. This is a quick way to get started using the API. To use this class, you need to write a class that inherits from it and implement any slots defined, which are marked as pure virtual in the generated header file. You need to add the necessary logic to manage the exposed properties and emit signals, to your class as well.



If you need to hide your implementation details, use the second class declared in the header file, the <Type>Source class. This class' definition does not provide data members, and also makes the getter/setter functions pure virtual. While you may need to write more code, using this class gives you more flexibility in your implementation.



The third class generated in the header is <Type>SourceAPI. This is a templated class, for use specifically by the templated version of [QRemoteObjectHostBase::enableRemoting](https://doc.qt.io/qt-5/qremoteobjecthostbase.html#enableRemoting)(), which allows you to use any [QObject](https://doc.qt.io/qt-5/qobject.html) that supports the required API as the source. Use this class to hide or convert properties or signal/slot parameters. If your implementation does not provide the correct API, there will be compile-time warnings.

**Note:** Replicas and sources both derive from [QObject](https://doc.qt.io/qt-5/qobject.html); but their [QObject](https://doc.qt.io/qt-5/qobject.html) API is **never** exposed. For instance, while a replica has a [destroyed](https://doc.qt.io/qt-5/qobject.html#destroyed) signal, the source's [destroyed](https://doc.qt.io/qt-5/qobject.html#destroyed) signal is not propagated. The source and each of its replica are unique QObjects with their own connections and lifetimes. Instead, the API that is exposed is defined by the `.rep` template used by repc. In the case of raw QObjects, all API elements are defined in the inheritance chain from a specific ancestor. The [QObject](https://doc.qt.io/qt-5/qobject.html)'s parent is always used, unless you define `Q_CLASSINFO("RemoteObject Type")` in an ancestor. If you use `Q_CLASSINFO("RemoteObject Type")`, that class's API is the lowest level of API used.



## Identifying Sources

Because a host node can share more than one source, each source requires a name. All repc-generated headers include a way for the node to determine the class name: [Q_CLASSINFO](https://doc.qt.io/qt-5/qobject.html#Q_CLASSINFO) for replica, simplesource, or source types; or a static `name()` function for the SourceAPI type. If you pass your own [QObject](https://doc.qt.io/qt-5/qobject.html) type to [QRemoteObjectHostBase::enableRemoting](https://doc.qt.io/qt-5/qremoteobjecthostbase.html#enableRemoting)(), the name is determined using the following logic:

- A name can optionally be passed to [QRemoteObjectHostBase::enableRemoting](https://doc.qt.io/qt-5/qremoteobjecthostbase.html#enableRemoting)(). If provided, that name is used.
- If the object or any of its ancestors has [Q_CLASSINFO](https://doc.qt.io/qt-5/qobject.html#Q_CLASSINFO) of type "RemoteObject Type" defined, that type name is used.
- If the [QObject](https://doc.qt.io/qt-5/qobject.html)'s objectName is set, then it is used.
- If none of the above are available, the call to [QRemoteObjectHostBase::enableRemoting](https://doc.qt.io/qt-5/qremoteobjecthostbase.html#enableRemoting)() fails, returning false.



© 2023 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.