#ifndef GSTARLOCALSOCKET_P_H
#define GSTARLOCALSOCKET_P_H

#include <QtCore/private/qobject_p.h>

#include "gstarlocalsocket.h"

class GStarLocalSocketPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(GStarLocalSocket)

public:
    GStarLocalSocketPrivate();
    virtual ~GStarLocalSocketPrivate();

public:
    QPointer<QLocalSocket> socket;
public:
    void onSocketDestroyed();
};

#endif // GSTARLOCALSOCKET_P_H

