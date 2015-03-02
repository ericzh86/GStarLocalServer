#ifndef GSTARLOCALSERVER_P_H
#define GSTARLOCALSERVER_P_H

#include <QtCore/private/qobject_p.h>

#include "gstarlocalserver.h"
#include "gstarlocalheader.h"

class GStarLocalServerPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(GStarLocalServer)

public:
    GStarLocalServerPrivate();
    virtual ~GStarLocalServerPrivate();

public:
    QPointer<QLocalServer> server;
    QScopedArrayPointer<char> buffer;

public:
    void _q_destroyed();
    void _q_newConnection();
    void _q_error(QLocalSocket::LocalSocketError);
    void _q_readyRead();

public:
    QHash<QLocalSocket *, GStarChunkHeader> chunkHeaders;

    QHash<SocketId, QLocalSocket *> connections;
    QHash<QLocalSocket *, SocketId> nameMapping;

public:
    void dispatch(QLocalSocket *sender, const GStarChunkHeader &chunkHeader);
    void post(QLocalSocket *socket, const GStarChunkHeader &chunkHeader);
    void removeSocket(QLocalSocket *socket);

public:
    quint16 idGenerator;
};

#endif // GSTARLOCALSERVER_P_H
