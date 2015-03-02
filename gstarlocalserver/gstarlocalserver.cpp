#include "gstarlocalserver.h"
#include "gstarlocalserver_p.h"

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

    if (d->server == server) {
        return;
    }
}

QLocalServer *GStarLocalServer::server() const
{
    Q_D(const GStarLocalServer);

    return d->server.data();
}

// class GStarLocalServerPrivate

GStarLocalServerPrivate::GStarLocalServerPrivate()
{
}

GStarLocalServerPrivate::~GStarLocalServerPrivate()
{
}

void GStarLocalServerPrivate::onServerDestroyed()
{
    Q_Q(GStarLocalServer);

    QLocalServer *server
            = qobject_cast<QLocalServer *>(q->sender());
    Q_ASSERT_X(server != 0, "GStarLocalServer", "Invalid sender object.");
}

// class moc_gstarlocalserver.cpp
#include "moc_gstarlocalserver.cpp"
