#include "gstarlocalserver.h"
#include "gstarlocalserver_p.h"

#include <QtCore/QLoggingCategory>

#include "gstarlocalglobal.h"

Q_LOGGING_CATEGORY(qlcStarLocalServer, "GStarLocalServer")

// class GStarLocalServer

GStarLocalServer::GStarLocalServer(QObject *parent)
    : QObject(*new GStarLocalServerPrivate(), parent)
{
    setServer(qobject_cast<QLocalServer *>(parent));
}

GStarLocalServer::GStarLocalServer(QLocalServer *server, QObject *parent)
    : QObject(*new GStarLocalServerPrivate(), parent)
{
    setServer(server);
}

void GStarLocalServer::setServer(QLocalServer *server)
{
    Q_D(GStarLocalServer);

    if (server == d->server) {
        return;
    }

    // 处理旧服务器信息.
    QLocalServer *oldServer = d->server.data();
    if (0 != oldServer) {
        disconnect(oldServer, SIGNAL(destroyed()),
                   this, SLOT(_q_destroyed()));
        disconnect(oldServer, SIGNAL(newConnection()),
                   this, SLOT(_q_newConnection()));
    }

    // 设置智能指针.
    d->server = server;

    // 处理新服务器信息.
    QLocalServer *newServer = d->server.data();
    if (0 != newServer) {
        connect(newServer, SIGNAL(destroyed()),
                this, SLOT(_q_destroyed()));
        connect(newServer, SIGNAL(newConnection()),
                this, SLOT(_q_newConnection()));
    }
}

QLocalServer *GStarLocalServer::server() const
{
    Q_D(const GStarLocalServer);

    return d->server.data();
}

// class GStarLocalServerPrivate

GStarLocalServerPrivate::GStarLocalServerPrivate()
    : idGenerator(1)
{
}

GStarLocalServerPrivate::~GStarLocalServerPrivate()
{
}

void GStarLocalServerPrivate::_q_destroyed()
{
    Q_Q(GStarLocalServer);

    QLocalServer *server
            = qobject_cast<QLocalServer *>(q->sender());
    Q_ASSERT_X(0 != server, "GStarLocalServer", "Invalid sender object.");
}

void GStarLocalServerPrivate::_q_newConnection()
{
    Q_Q(GStarLocalServer);

    while (server->hasPendingConnections()) {
        QLocalSocket *newSocket = server->nextPendingConnection();
        if (0 != newSocket) {
            QObject::connect(newSocket, SIGNAL(error(QLocalSocket::LocalSocketError)),
                             q, SLOT(_q_error(QLocalSocket::LocalSocketError)));
            QObject::connect(newSocket, SIGNAL(readyRead()),
                             q, SLOT(_q_readyRead()));

            for (; ; ++idGenerator) {
                if ((idGenerator != 0) && !connections.contains(idGenerator)) {
                    qCDebug(qlcStarLocalServer) << "New connection:" << idGenerator;
                    connections.insert(idGenerator, newSocket);
                    nameMapping.insert(newSocket, idGenerator);
                    ++idGenerator;
                    break;
                }
            }
        }
    }
}

void GStarLocalServerPrivate::_q_error(QLocalSocket::LocalSocketError)
{
    Q_Q(GStarLocalServer);

    QLocalSocket *socket
            = qobject_cast<QLocalSocket *>(q->sender());
    Q_ASSERT_X(0 != socket, "GStarLocalServer", "Invalid sender object.");

    removeSocket(socket);
}

void GStarLocalServerPrivate::_q_readyRead()
{
    Q_Q(GStarLocalServer);

    QLocalSocket *socket
            = qobject_cast<QLocalSocket *>(q->sender());
    Q_ASSERT_X(0 != socket, "GStarLocalServer", "Invalid sender object.");

    if (buffer.isNull()) {
        buffer.reset(new char[MAX_CHUNK_SIZE]);
    }

    GStarChunkHeader &chunkHeader = chunkHeaders[socket];

    while (socket->bytesAvailable()) {
        if (chunkHeader.isValid()) {
            if (socket->bytesAvailable() >= chunkHeader.chunkSize()) {
                const qint64 actualSize = socket->read(buffer.data(), chunkHeader.chunkSize());
                if (chunkHeader.chunkSize() != actualSize) {
                    removeSocket(socket);
                    return;
                }
                dispatch(socket, chunkHeader);
                chunkHeader.reset();
            } else {
                break;
            }
        } else {
            if (socket->bytesAvailable() >= chunkHeader.size()) {
                const qint64 actualSize = socket->read(chunkHeader.data(), chunkHeader.size());
                if (chunkHeader.size() != actualSize) {
                    removeSocket(socket);
                    return;
                }
            } else {
                break;
            }
        }
    }
}

void GStarLocalServerPrivate::dispatch(QLocalSocket *sender, const GStarChunkHeader &chunkHeader)
{
    if (chunkHeader.protocol() == GStarChunkHeader::PostChunkData) {
        SocketId senderId = nameMapping.value(sender);
        if (!senderId) {
            qCDebug(qlcStarLocalServer) << "Invalid sender id.";
            return;
        }

        GStarChunkHeader postChunkHeader;
        postChunkHeader.set(senderId, chunkHeader.protocol(),
                            chunkHeader.chunkSize(), chunkHeader.chunkIndex());

        if (chunkHeader.targetId()) {
            QLocalSocket *socket = connections.value(chunkHeader.targetId());
            if (socket) {
                post(socket, postChunkHeader);
            }
        } else {
            Q_FOREACH(QLocalSocket *socket, connections) {
                if (sender == socket) {
                    post(socket, postChunkHeader);
                }
            }
        }
    } else if (chunkHeader.protocol() == GStarChunkHeader::GetAllSocketIds) {
        ;
    }
}

void GStarLocalServerPrivate::post(QLocalSocket *socket, const GStarChunkHeader &chunkHeader)
{
    qint64 actualSize = socket->write(chunkHeader.constData(), chunkHeader.size());
    if (chunkHeader.size() != actualSize) {
        removeSocket(socket);
        return;
    }

    actualSize = socket->write(buffer.data(), chunkHeader.chunkSize());
    if (chunkHeader.chunkSize() != actualSize) {
        removeSocket(socket);
        return;
    }
}

void GStarLocalServerPrivate::removeSocket(QLocalSocket *socket)
{
    qCDebug(qlcStarLocalServer) << socket->error();

    socket->close();
    chunkHeaders.remove(socket);
    connections.remove(nameMapping.take(socket));
}

// class moc_gstarlocalserver.cpp
#include "moc_gstarlocalserver.cpp"
