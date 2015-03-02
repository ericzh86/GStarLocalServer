#ifndef GSTARLOCALSERVER_H
#define GSTARLOCALSERVER_H

#include <QtCore/QObject>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

#include "gstarlocalglobal.h"

class GStarLocalServerPrivate;
class GStarLocalServer : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GStarLocalServer)

public:
    explicit GStarLocalServer(QObject *parent = 0);
    explicit GStarLocalServer(QLocalServer *server, QObject *parent = 0);

Q_SIGNALS:
    void serverChanged(QLocalServer *server);
public:
    void setServer(QLocalServer *server);
    QLocalServer *server() const;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_destroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_newConnection())
    Q_PRIVATE_SLOT(d_func(), void _q_error(QLocalSocket::LocalSocketError))
    Q_PRIVATE_SLOT(d_func(), void _q_readyRead())
};

#endif // GSTARLOCALSERVER_H
