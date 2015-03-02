#ifndef GSTARLOCALSOCKET_H
#define GSTARLOCALSOCKET_H

#include <QtCore/QObject>
#include <QtNetwork/QLocalSocket>

class GStarLocalSocketPrivate;
class GStarLocalSocket : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GStarLocalSocket)

public:
    explicit GStarLocalSocket(QObject *parent = 0);
    explicit GStarLocalSocket(QLocalSocket *socket, QObject *parent = 0);

public:
    void setSocket(QLocalSocket *socket);
    QLocalSocket *socket() const;

private:
    Q_PRIVATE_SLOT(d_func(), void onSocketDestroyed())
};

#endif // GSTARLOCALSOCKET_H
