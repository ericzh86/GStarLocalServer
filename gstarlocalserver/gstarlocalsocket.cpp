#include "gstarlocalsocket.h"
#include "gstarlocalsocket_p.h"

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

    if (d->socket == socket) {
        return;
    }
}

QLocalSocket *GStarLocalSocket::socket() const
{
    Q_D(const GStarLocalSocket);

    return d->socket.data();
}

// class GStarLocalSocketPrivate

GStarLocalSocketPrivate::GStarLocalSocketPrivate()
{
}

GStarLocalSocketPrivate::~GStarLocalSocketPrivate()
{
}

void GStarLocalSocketPrivate::onSocketDestroyed()
{
    Q_Q(GStarLocalSocket);

    QLocalSocket *socket
            = qobject_cast<QLocalSocket *>(q->sender());
    Q_ASSERT_X(socket != 0, "GStarLocalSocket", "Invalid sender object.");
}

// class moc_gstarlocalsocket.cpp
#include "moc_gstarlocalsocket.cpp"
