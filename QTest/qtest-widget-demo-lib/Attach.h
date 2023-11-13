#ifndef ATTACH_H
#define ATTACH_H

#include <QObject>
#include <QtDebug>

namespace swp {
class Attach : public QObject
{
    Q_OBJECT
public:
    Attach()=default;
    ~Attach()=default;
    static void AA();
signals:

};
}

#endif // ATTACH_H
