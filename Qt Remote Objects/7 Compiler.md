# Qt Remote Objects Compiler





## REPC Overview

The Replica Compiler (repc) generates [QObject](https://doc.qt.io/qt-5/qobject.html) header files based on an API definition file. The file (called a "rep" file) uses a specific (text) syntax to describe the API. By convention, these files are given a .rep file extension, short for Replica. When these files are processed by repc, repc generates both [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) and [Replica](https://doc.qt.io/qt-5/qtremoteobjects-replica.html#replica) header files.

The Qt Remote Objects module also includes qmake variables ([REPC_SOURCE](https://doc.qt.io/qt-5/qtremoteobjects-repc.html#repc-source), [REPC_REPLICA](https://doc.qt.io/qt-5/qtremoteobjects-repc.html#repc-replica), and [REPC_MERGED](https://doc.qt.io/qt-5/qtremoteobjects-repc.html#repc-merged)) that can be added to your project file to automatically run repc, and add the resulting files to the list of files processed by [Meta Object Compiler](https://doc.qt.io/qt-5/moc.html) during the build process, making use of Qt Remote Objects in your projects simple.

While Qt Remote Objects supports sharing any [QObject](https://doc.qt.io/qt-5/qobject.html) over the network (using enableRemoting on the Source side and acquireDynamic on the Replica side), there are a couple of advantages to letting repc define your objects. First of all, while [DynamicReplicas](https://doc.qt.io/qt-5/qremoteobjectdynamicreplica.html) are useful, they are more cumbersome to work with. The API is not known until the object is initialized, and using the API from C++ requires string lookups through [QMetaObject](https://doc.qt.io/qt-5/qmetaobject.html)'s methods. Secondly, having the interface known at compile time finds any issues at compile vs. at runtime. Thirdly, the rep format supports default values, which can be handy if you are unable to ensure the Source is available when the Replica is instantiated.

See the documentation [here](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) for information on using the generated files in your code. Here we will focus on the repc format and options.



## The rep file format

The rep file format is a simple [Domain Specific Language (DSL)](https://en.wikipedia.org/wiki/Domain-specific_language) for describing an interface supported over Qt Remote Objects (QtRO). Since QtRO is an object based system, these interfaces are defined by APIs available through objects, that is, classes with properties, signals, and slots.



### The class type

Each class defined in a rep file becomes a [QObject](https://doc.qt.io/qt-5/qobject.html) in the generated header files, with the described API generated for you.

To define a class use the `class` keyword, followed by the name you want for your type, and then enclose your API in brackets like so

```
class MyType
{
    //PROP/CLASS/MODEL/SIGNAL/SLOT/ENUM declarations to define your API
};
```



#### PROP

[Q_PROPERTY](https://doc.qt.io/qt-5/qobject.html#Q_PROPERTY) elements are created by using the PROP keyword in the rep file. The syntax is the `PROP` keyword followed by the definition enclosed in parentheses, where the definition is the type, the name, and (optionally) a default value or attributes.

```
PROP(bool simpleBool)                // boolean named simpleBool
PROP(bool defaultFalseBool=false)    // boolean named defaultFalseBool, with false
                                     // as the default value

PROP(int lifeUniverseEverything=42)  // int value that defaults to 42
PROP(QByteArray myBinaryInfo)        // Qt types are fine, may need #include
                                     // additional headers in your rep file

PROP(QString name CONSTANT)          // Property with the CONSTANT attribute
PROP(QString setable READWRITE)      // Property with the READWRITE attribute
                                     // note: Properties default to READPUSH
                                     // (see description below)

PROP(SomeOtherType myCustomType)     // Custom types work. Needs #include for the
                                     // appropriate header for your type, make
                                     // sure your type is known to the metabject
                                     // system, and make sure it supports Queued
                                     // Connections (see Q_DECLARE_METATYPE and
                                     // qRegisterMetaType)
```

More information about creating custom types can be found [here](https://doc.qt.io/qt-5/custom-types.html).

By default, properties will have getters and a "push" slot defined, as well as a notify signal emitted when the value is changed. Qt Remote Objects requires the notify signal on the Source object to trigger sending updates to the attached Replicas. In earlier versions of QtRO, properties defaulted to being read/write, that is, having getters and setters. However, due to the asynchronous nature of QtRO, this led to unintuitive behavior at times. Setting the READWRITE attribute on the PROP will provide the old (getter and setter) behavior.

```
// In .rep file, old (setter) behavior
PROP(int myVal READWRITE)             // Old behavior with setMyVal(int myVal) method

// In code...  Assume myVal is initially set to 0 in Source
int originalValue = rep->myVal();     // Will be 0
rep->setMyVal(10);                    // Call setter, expecting a blocking/
                                      // non-asynchronous return

if (rep->myVal() == 10) ...           // Test will usually fail
```

If it is necessary to block until the value is changed, something like the following is necessary.

```
// In .rep file, old (setter) behavior
PROP(int myVal READWRITE)             // Old behavior with setMyVal(int myVal) method

// In code...  Assume myVal is initially set to 0 in Source
bool originalValue = rep->myVal();    // Will be 0

// We can wait for the change using \l QSignalSpy
QSignalSpy spy(rep, SIGNAL(myValChanged(int)));

rep->setMyVal(10);                    // Call setter, expecting a blocking/
                                      // non-asynchronous return

spy.wait();                           // spy.wait() blocks until changed signal
                                      // is received
if (rep->myVal() == 10) ...           // Test will succeed assuming
                                      // 1. Source object is connected
                                      // 2. Nobody else (Source or other Replica)
                                      //    sets the myVal to something else (race
                                      //    condition)
// Rather than use QSignalSpy, the event-driven practice would be to connect the
// myValChanged notify signal to a slot that responds to the changes.
```

QtRO now defaults to READPUSH, which provides an automatically generated slot for requesting a property change.

```
// In .rep file, defaults to READPUSH
PROP(bool myVal)                      // No setMyVal(int myVal) on Replica, has
                                      // pushMyVal(int myVal) instead

// In code...  Assume myVal is initially set to 0 in Source
bool originalValue = rep->myVal();    // Will be 0

// We can wait for the change using \l QSignalSpy
QSignalSpy spy(rep, SIGNAL(myValChanged(int)));

rep->pushMyVal(10);                   // Call push method, no expectation that change
                                      // is applied upon method completion.

// Some way of waiting for change to be received by the Replica is still necessary,
// but hopefully not a surprise with the new pushMyVal() Slot.
spy.wait();                           // spy.wait() blocks until changed signal
                                      // is received
if (rep->myVal() == 10) ...           // Test will succeed assuming
                                      // 1. Source object is connected
                                      // 2. Nobody else (Source or other Replica)
                                      //    set the myVal to something else (race
                                      //    condition)
```

You can also use the `CONSTANT`, `READONLY`, `PERSISTED`, `READWRITE`, `READPUSH`, or `SOURCEONLYSETTER` keywords in the PROP declaration, which affects how the property is implemented. READPUSH is the default value if no value used.

```
PROP(int lifeUniverseEverything=42 CONSTANT)
PROP(QString name READONLY)
```

Please note there are some subtleties here. A CONSTANT PROP has a [Q_PROPERTY](https://doc.qt.io/qt-5/qobject.html#Q_PROPERTY) declared as CONSTANT on the SOURCE side. However, replicas cannot know the correct value until they are initialized, which means the property value has to be allowed to change during initialization. For READONLY, the source will have neither a setter nor a push slot, and the replica side will not have a push slot generated. Adding the PERSISTED trait to a PROP will have the PROP use the [QRemoteObjectAbstractPersistedStore](https://doc.qt.io/qt-5/qremoteobjectabstractpersistedstore.html) instance set on a Node (if any) to save/restore PROP values.

Another nuanced value is SOURCEONLYSETTER, which provides another way of specifying asymmetric behavior, where the [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) (specifically the helper class, `SimpleSource`) will have a public getter and setter for the property, but it will be ReadOnly (with a notify signal) on the [Replica](https://doc.qt.io/qt-5/qtremoteobjects-replica.html#replica) side. Thus the property can be fully controlled from the [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) side, but only observed from the [Replica](https://doc.qt.io/qt-5/qtremoteobjects-replica.html#replica) side. SOURCEONLYSETTER is the mode used by repc for MODEL and CLASS instances, meaning the [Source](https://doc.qt.io/qt-5/qtremoteobjects-source.html#source) can change the pointed to object, but the [Replica](https://doc.qt.io/qt-5/qtremoteobjects-replica.html#replica) can not provide a new object, as no set<Prop> or push<Prop> method is generated. Note, this does not impact the behavior of the pointed to type's properties, just the ability to change the pointer itself.



#### CLASS

The CLASS keyword generates special [Q_PROPERTY](https://doc.qt.io/qt-5/qobject.html#Q_PROPERTY) elements for objects derived from [QObject](https://doc.qt.io/qt-5/qobject.html). These properties have the same semantics as SOURCEONLYSETTER. The syntax is the `CLASS` keyword followed by the property name and then the type of subobject enclosed in parentheses.

```
// In .rep file
class OtherClass
{
    PROP(int value)
}

class MainClass
{
    CLASS subObject(OtherClass)
}
```



#### MODEL

The MODEL keyword generates special [Q_PROPERTY](https://doc.qt.io/qt-5/qobject.html#Q_PROPERTY) elements for objects derived from [QAbstractItemModel](https://doc.qt.io/qt-5/qabstractitemmodel.html). These properties have the same semantics as SOURCEONLYSETTER. The syntax is the `MODEL` keyword followed by the property name and then parentheses enclosing the (comma-separated) roles that should be exposed to the replica.

```
// In .rep file
class CdClass
{
    PROP(QString title READONLY)
    MODEL tracks(title, artist, length)
}
```



#### SIGNAL

[Signal](https://doc.qt.io/qt-5/signalsandslots.html#signals) methods are created by using the SIGNAL keyword in the rep file.

Usage is to declare `SIGNAL` followed by the desired signature wrapped in parentheses. The void return value should be skipped.

```
SIGNAL(test())
SIGNAL(test(QString foo, int bar))
SIGNAL(test(QMap<QString,int> foo))
SIGNAL(test(const QString &foo))
SIGNAL(test(QString &foo))
```

Just like in Qt [queued connections](https://doc.qt.io/qt-5/qt.html#ConnectionType-enum), parameters in signals that are references will be copied when being passed to replicas.



#### SLOT

[Slot](https://doc.qt.io/qt-5/signalsandslots.html#slots) methods are created by using the SLOT keyword in the rep file.

Usage is to declare `SLOT` followed by the desired signature wrapped in parentheses. The return value can be included in the declaration. If the return value is skipped, void will be used in the generated files.

```
SLOT(test())
SLOT(void test(QString foo, int bar))
SLOT(test(QMap<QString,int> foo))
SLOT(test(QMap<QString,int> foo, QMap<QString,int> bar))
SLOT(test(QMap<QList<QString>,int> foo))
SLOT(test(const QString &foo))
SLOT(test(QString &foo))
SLOT(test(const QMap<QList<QString>,int> &foo))
SLOT(test(const QString &foo, int bar))
```

Just like in Qt [queued connections](https://doc.qt.io/qt-5/qt.html#ConnectionType-enum) and QtRO SIGNALS, parameters in slots that are references will be copied when being passed to Replicas.



#### ENUM

Enumerations (which use a combination of C++ enum and Qt's [Q_ENUM](https://doc.qt.io/qt-5/qobject.html#Q_ENUM) in QtRO) are described using the ENUM keyword.

```
ENUM MyEnum {Foo}
ENUM MyEnum {Foo, Bar}
ENUM MyEnum {Foo, Bar = -1}
ENUM MyEnum {Foo=-1, Bar}
ENUM MyEnum {Foo=0xf, Bar}
ENUM MyEnum {Foo=1, Bar=3, Bas=5}
```

Related topics: [The ENUM type](https://doc.qt.io/qt-5/qtremoteobjects-repc.html#the-enum-type), [USE_ENUM keyword](https://doc.qt.io/qt-5/qtremoteobjects-repc.html#use-enum-keyword)



### The POD type

Plain Old Data (POD) is a term to describe a simple data collection, along the lines of a C++ struct. For example, if you have an API for a phonebook, you may want to use the concept of an "address" in its interface (where address might include street, city, state, country, and postal code). You can use the POD keyword to define objects like this, which can then be used by in PROP/SIGNAL/SLOT definitions in your class definitions.

Usage is to declare `POD` followed by the name for the generated type, followed by type and name pairs separated by commas, where the type/name pairs are wrapped in parentheses.

```
POD Foo(int bar)
POD Foo(int bar, double bas)
POD Foo(QMap<QString,int> bar)
POD Foo(QList<QString> bar)
POD Foo(QMap<QString,int> bar, QMap<double,int> bas)
```

A full example would look like this

```
POD Foo(QList<QString> bar)
class MyType
{
    SIGNAL(sendCustom(Foo foo));
};
```

The code generated by repc creates a [Q_GADGET](https://doc.qt.io/qt-5/qobject.html#Q_GADGET) class for each POD, with corresponding [Q_PROPERTY](https://doc.qt.io/qt-5/qobject.html#Q_PROPERTY) members for each type defined for the POD.



### The ENUM type

It is often easier and cleaner to define an ENUM inside a class (see [ENUM](https://doc.qt.io/qt-5/qtremoteobjects-repc.html#enum)), but if you need a standalone enum type, using the ENUM keyword outside of a class definition can be helpful. This will generate a new class in your header files that handles marshalling, etc.. The syntax is identical to [ENUM](https://doc.qt.io/qt-5/qtremoteobjects-repc.html#enum), with the exception that the declaration in this case is not contained in a `class` declaration.

Related topics: [ENUM](https://doc.qt.io/qt-5/qtremoteobjects-repc.html#enum), [USE_ENUM keyword](https://doc.qt.io/qt-5/qtremoteobjects-repc.html#use-enum-keyword)



### USE_ENUM keyword

The USE_ENUM keyword was implemented before autogeneration via the ENUM keyword was added. It is kept for backwards compatibility.

Related topics: [ENUM](https://doc.qt.io/qt-5/qtremoteobjects-repc.html#enum), [The ENUM type](https://doc.qt.io/qt-5/qtremoteobjects-repc.html#the-enum-type)



### Directives

The rep file defines an interface, but interfaces often require external elements. In order to support this, repc will include any (single line) directives at the top of the generated files. This allows you to, for instance, use #include or #define directives that support the logic or datatypes needed.

The repc tool currently ignores everything from the "#" symbol to the end-of-line and adds that to the generated files. So multi-line #if/#else/#endif statements and multi-line macros are not supported.



## qmake variables



### REPC_REPLICA

Specifies the names of all rep files in the project that should be used to generate replica header files.

For example:

```
REPC_REPLICA = media.rep \
               location.rep
```

The generated file(s) will be of the form `rep_<replica file base>_replica.h`.



### REPC_SOURCE

Specifies the names of all rep files in the project that should be used to generate source header files.

For example:

```
REPC_SOURCE = media.rep \
              location.rep
```

The generated file(s) will be of the form `rep_<replica file base>_source.h`.



### REPC_MERGED

Specifies the names of all rep files in the project that should be used to generate combined (source and replica) header files.

For example:

```
REPC_MERGED = media.rep \
              location.rep

The generated file(s) will be of the form \c {rep_<replica file base>_merged.h}.

\note Typically sources and replicas live in separate processes or devices, so this variable
is not commonly used.
```



### QOBJECT_REP

Specifies the names of existing [QObject](https://doc.qt.io/qt-5/qobject.html) header files that should be used to generate corresponding .rep files.

**See also** [QRemoteObjectAbstractPersistedStore](https://doc.qt.io/qt-5/qremoteobjectabstractpersistedstore.html).



© 2023 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.