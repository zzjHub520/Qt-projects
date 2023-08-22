# Qt Remote Objects Protocol Versioning





Qt Remote Objects uses an internal protocol to pass data between processes and/or devices. The same protocol version needs to be used by all parties: if there is a mismatch, the connecting node will output a warning and the host node will not send any data.

Currently released versions:

| Protocol Version |               Qt Version               |
| :--------------: | :------------------------------------: |
|       <1.2       | Used during QtRO's tech preview phase. |
|       1.2        |                 5.12.0                 |
|       1.3        |                 5.12.4                 |



© 2023 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.