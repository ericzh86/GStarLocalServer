#include <QString>
#include <QtTest>

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
    QVERIFY2(true, "Failure");
}

QTEST_GUILESS_MAIN(GStarLocalServerTest)

#include "tst_gstarlocalserver.moc"
