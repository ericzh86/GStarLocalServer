#ifndef GSTARLOCALSOCKET_P_H
#define GSTARLOCALSOCKET_P_H

#include <QtCore/private/qobject_p.h>

#include "gstarlocalsocket.h"
#include "gstarlocalheader.h"

class GStarLocalSocketPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(GStarLocalSocket)

public:
    GStarLocalSocketPrivate();
    virtual ~GStarLocalSocketPrivate();

public:
    QPointer<QLocalSocket> socket;
    QScopedArrayPointer<char> buffer;

public:
    void _q_destroyed();
    void _q_error(QLocalSocket::LocalSocketError socketError);
    void _q_readyRead();

public:
    GStarChunkHeader chunkHeader;
    QHash<quint16, QVector<QByteArray> > chunkBuffers;

public:
    void dispatch();
};

#endif // GSTARLOCALSOCKET_P_H

