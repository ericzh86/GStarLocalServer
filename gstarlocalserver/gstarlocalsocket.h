#ifndef GSTARLOCALSOCKET_H
#define GSTARLOCALSOCKET_H

#include <QtCore/QObject>
#include <QtNetwork/QLocalSocket>

#include "gstarlocalglobal.h"

class GStarLocalSocketPrivate;
class GStarLocalSocket : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GStarLocalSocket)

public:
    explicit GStarLocalSocket(QObject *parent = 0);
    explicit GStarLocalSocket(QLocalSocket *socket, QObject *parent = 0);

Q_SIGNALS:
    void socketChanged(QLocalSocket *socket);
public:
    void setSocket(QLocalSocket *socket);
    QLocalSocket *socket() const;

Q_SIGNALS:
    void received(const QVector<QByteArray> &chunks);
public:
    bool post(const QByteArray &data);
    bool post(const char *data, qint64 size);

    bool post(SocketId target, const QByteArray &data);
    bool post(SocketId target, const char *data, qint64 size);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_destroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_error(QLocalSocket::LocalSocketError))
    Q_PRIVATE_SLOT(d_func(), void _q_readyRead())
};

#endif // GSTARLOCALSOCKET_H
