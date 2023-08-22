## Remote Object Interaction

The interaction between source and replicas is directional. Property changes and signal emission happen on the source, and are propagated to all replicas. If a property is writable, you can call the setter function on a replica, which is then forwarded to the source. Afterwards, if this call results in a new property value, that value is first changed at the source and then subsequently forwarded to all replicas. To the replica, it is then an asynchronous call, with latency before the change takes effect.

While technically you can emit a signal on a replica, this is discouraged as it may have unexpected results. It will only trigger slots connected to the replica itself; not slots connected to the source or other replicas. Like property setters, slot invocations on a replica are forwarded to the source to run.

Qt Remote Objects automatically implements the behavior described above. There is no need to write any replica implementation code. For dynamic replicas, this is handled automatically at runtime. For [repc](https://doc.qt.io/qt-5/qtremoteobjects-repc.html) generated headers, this is handled at compile time.



© 2023 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.