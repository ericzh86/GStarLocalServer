#ifndef GSTARLOCALSERVER_H
#define GSTARLOCALSERVER_H

#include <QtCore/QObject>
#include <QtNetwork/QLocalServer>

class GStarLocalServerPrivate;
class GStarLocalServer : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GStarLocalServer)

public:
    explicit GStarLocalServer(QObject *parent = 0);
    explicit GStarLocalServer(QLocalServer *server, QObject *parent = 0);

public:
    void setServer(QLocalServer *server);
    QLocalServer *server() const;

private:
    Q_PRIVATE_SLOT(d_func(), void onServerDestroyed())
};

#endif // GSTARLOCALSERVER_H
