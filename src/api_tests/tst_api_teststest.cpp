#include <QString>
#include <QtTest>
#include "Request.h"
#include "Watcher.h"
#include "Parser.h"
#include "server.h"
#include <iostream>
#include "response.h"
#include <thread>
#include <unistd.h>
#include <future>
#include "dispatcher.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
class Api_testsTest : public QObject
{
    Q_OBJECT

public:
    Api_testsTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void ChromeOnce();
};

Api_testsTest::Api_testsTest()
{
}

void Api_testsTest::initTestCase()
{

}

void Api_testsTest::cleanupTestCase()
{

}

void Api_testsTest::ChromeOnce()
{
    try {
    Web::Server s(1234);
    Web::Dispatcher::routes.insert(std::make_pair("/adsaf",[](Http::Request req) -> Http::Response {

                                       return {req, 200};
                                   }));
    s.run();
    }
    catch(std::exception &ex) {
        std::cerr << ex.what();
    }
}


QTEST_APPLESS_MAIN(Api_testsTest)

#include "tst_api_teststest.moc"
