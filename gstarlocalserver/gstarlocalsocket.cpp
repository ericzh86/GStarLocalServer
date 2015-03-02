#include "gstarlocalsocket.h"
#include "gstarlocalsocket_p.h"

#include <QtCore/QLoggingCategory>

#include "gstarlocalglobal.h"

Q_LOGGING_CATEGORY(qlcStarLocalSocket, "GStarLocalSocket")

// class GStarLocalSocket

GStarLocalSocket::GStarLocalSocket(QObject *parent)
    : QObject(*new GStarLocalSocketPrivate(), parent)
{
    setSocket(qobject_cast<QLocalSocket *>(parent));
}

GStarLocalSocket::GStarLocalSocket(QLocalSocket *socket, QObject *parent)
    : QObject(*new GStarLocalSocketPrivate(), parent)
{
    setSocket(socket);
}

void GStarLocalSocket::setSocket(QLocalSocket *socket)
{
    Q_D(GStarLocalSocket);

    if (socket == d->socket) {
        return;
    }

    // 处理旧服务器信息.
    QLocalSocket *oldSocket = d->socket.data();
    if (0 != oldSocket) {
        disconnect(oldSocket, SIGNAL(destroyed()),
                   this, SLOT(_q_destroyed()));
        disconnect(oldSocket, SIGNAL(error(QLocalSocket::LocalSocketError)),
                   this, SLOT(_q_error(QLocalSocket::LocalSocketError)));
        disconnect(oldSocket, SIGNAL(readyRead()),
                   this, SLOT(_q_readyRead()));
    }

    // 设置智能指针.
    d->socket = socket;

    // 处理新服务器信息.
    QLocalSocket *newSocket = d->socket.data();
    if (0 != newSocket) {
        connect(newSocket, SIGNAL(destroyed()),
                this, SLOT(_q_destroyed()));
        connect(newSocket, SIGNAL(error(QLocalSocket::LocalSocketError)),
                this, SLOT(_q_error(QLocalSocket::LocalSocketError)));
        connect(newSocket, SIGNAL(readyRead()),
                this, SLOT(_q_readyRead()));
    }
}

QLocalSocket *GStarLocalSocket::socket() const
{
    Q_D(const GStarLocalSocket);

    return d->socket.data();
}

bool GStarLocalSocket::post(const QByteArray &data)
{
    return post(0, data.constData(), data.size());
}

bool GStarLocalSocket::post(const char *data, qint64 size)
{
    return post(0, data, size);
}

bool GStarLocalSocket::post(SocketId target, const QByteArray &data)
{
    return post(target, data.constData(), data.size());
}

bool GStarLocalSocket::post(SocketId target, const char *data, qint64 size)
{
    Q_D(GStarLocalSocket);

    if (!d->socket || !d->socket->isOpen()) {
        qCDebug(qlcStarLocalSocket) << "Invalid socket.";
        return false;
    }

    bool hasError = false;
    qint64 actualSize = 0;
    GStarChunkHeader chunkHeader;
    qint64 chunkCount = (size / MAX_CHUNK_SIZE) + 1;
    for (qint64 i = 0; i < chunkCount; ++i) {
        const qint64 chunkSize = qMin(size - (i * MAX_CHUNK_SIZE), MAX_CHUNK_SIZE);
        chunkHeader.set(target, GStarChunkHeader::PostChunkData, chunkSize, chunkCount - i - 1);
        actualSize = d->socket->write(chunkHeader.constData(), chunkHeader.size());
        if (chunkHeader.size() != actualSize) {
            hasError = true;
            break;
        }
        const char *chunk = &data[i * MAX_CHUNK_SIZE];
        actualSize = d->socket->write(chunk, chunkSize);
        if (chunkSize != actualSize) {
            hasError = true;
            break;
        }
    }

    if (hasError) {
        qCDebug(qlcStarLocalSocket) << d->socket->error();

        d->socket->close();
        d->chunkHeader.reset();

        return false;
    }

    return true;
}

// class GStarLocalSocketPrivate

GStarLocalSocketPrivate::GStarLocalSocketPrivate()
{
}

GStarLocalSocketPrivate::~GStarLocalSocketPrivate()
{
}

void GStarLocalSocketPrivate::_q_destroyed()
{
    Q_Q(GStarLocalSocket);

    QLocalSocket *socket
            = qobject_cast<QLocalSocket *>(q->sender());
    Q_ASSERT_X(socket != 0, "GStarLocalSocket", "Invalid sender object.");

    chunkHeader.reset();
}

void GStarLocalSocketPrivate::_q_error(QLocalSocket::LocalSocketError socketError)
{
    qCDebug(qlcStarLocalSocket) << socketError;

    socket->close();
    chunkHeader.reset();
}

void GStarLocalSocketPrivate::_q_readyRead()
{
    if (buffer.isNull()) {
        buffer.reset(new char[MAX_CHUNK_SIZE]);
    }

    bool hasError = false;
    while (socket->bytesAvailable()) {
        if (chunkHeader.isValid()) {
            if (socket->bytesAvailable() >= chunkHeader.chunkSize()) {
                const qint64 actualSize = socket->read(buffer.data(), chunkHeader.chunkSize());
                if (chunkHeader.chunkSize() != actualSize) {
                    hasError = true;
                    break;
                }
                dispatch();
                chunkHeader.reset();
            } else {
                break;
            }
        } else {
            if (socket->bytesAvailable() >= chunkHeader.size()) {
                const qint64 actualSize = socket->read(chunkHeader.data(), chunkHeader.size());
                if (chunkHeader.size() != actualSize) {
                    hasError = true;
                    break;
                }
            } else {
                break;
            }
        }
    }

    if (hasError) {
        qCDebug(qlcStarLocalSocket) << socket->error();

        socket->close();
        chunkHeader.reset();
    }
}

void GStarLocalSocketPrivate::dispatch()
{
    Q_Q(GStarLocalSocket);

    if (chunkHeader.protocol() == GStarChunkHeader::PostChunkData) {
        QVector<QByteArray> &chunkBuffer
                = chunkBuffers[chunkHeader.senderId()];
        chunkBuffer << QByteArray(buffer.data(), chunkHeader.chunkSize());
        if (chunkHeader.chunkIndex() == 0) {
            emit q->received(chunkBuffer);
            chunkBuffer.clear();
        }
    } else if (chunkHeader.protocol() == GStarChunkHeader::GetAllSocketIds) {
        ;
    }
}

// class moc_gstarlocalsocket.cpp
#include "moc_gstarlocalsocket.cpp"
