#include <QString>
#include <QtTest>
#include <QtNetwork>
#include <QtCore>

#include "gstarlocalserver/gstarlocalserver.h"
#include "gstarlocalserver/gstarlocalsocket.h"

class GStarLocalServerTest : public QObject
{
    Q_OBJECT

public:
    GStarLocalServerTest();

private Q_SLOTS:
    void testCase();
};

GStarLocalServerTest::GStarLocalServerTest()
{
}

void GStarLocalServerTest::testCase()
{
    QString serverName = QUuid::createUuid().toString();

    QByteArray test(0xFFFFFF, 0);

    QLocalServer server;
    if (!server.listen(serverName)) {
        qDebug("Listen failed.");
    }

    GStarLocalServer localServer;
    localServer.setServer(&server);

    QVERIFY(localServer.server() == &server);

    // socket1
    QLocalSocket socket1;
    socket1.connectToServer(serverName);
    socket1.waitForConnected();

    GStarLocalSocket localSocket1;
    localSocket1.setSocket(&socket1);

    QVERIFY(localSocket1.socket() == &socket1);

    // socket2
    QLocalSocket socket2;
    socket2.connectToServer(serverName);
    socket2.waitForConnected();

    GStarLocalSocket localSocket2;
    localSocket2.setSocket(&socket2);

    QVERIFY(localSocket2.socket() == &socket2);

    //
    localSocket1.post(test);

    QEventLoop el;
    QTimer::singleShot(5000, &el, SLOT(quit()));
    el.exec();
}

QTEST_GUILESS_MAIN(GStarLocalServerTest)

#include "tst_gstarlocalserver.moc"
