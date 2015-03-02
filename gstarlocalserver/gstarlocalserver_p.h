#ifndef GSTARLOCALSERVER_P_H
#define GSTARLOCALSERVER_P_H

#include <QtCore/private/qobject_p.h>

#include "gstarlocalserver.h"

class GStarLocalServerPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(GStarLocalServer)

public:
    GStarLocalServerPrivate();
    virtual ~GStarLocalServerPrivate();

public:
    QPointer<QLocalServer> server;
public:
    void onServerDestroyed();
};

#endif // GSTARLOCALSERVER_P_H

