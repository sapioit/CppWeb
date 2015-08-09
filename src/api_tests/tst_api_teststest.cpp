#include <QString>
#include <QtTest>
#include "Request.h"
#include "Watcher.h"
#include "HttpUtility.h"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <future>
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
    void SiegeOnce();
    void GoogleOnce();
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

void Api_testsTest::GoogleOnce()
{

    Http::Request expected;
    expected.method = Http::Request::Method::Get;
    expected.URI = "/adsaf";
    expected.version = "HTTP/1.1";
    expected.header.fields["Accept-Language"] = "ro-RO,ro;q=0.8,en-US;q=0.6,en;q=0.4,es;q=0.2,tr;q=0.2";
    expected.header.fields["Accept-Encoding"] = "gzip, deflate, sdch";
    expected.header.fields["User-Agent"] = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.130 Safari/537.36";
    expected.header.fields["Upgrade-Insecure-Requests"] = "1";
    expected.header.fields["Host"] = "192.168.2.105:1234";
    expected.header.fields["Cache-Control"] = "max-age=0";
    expected.header.fields["Connection"] = "keep-alive";
    expected.header.fields["Accept"] = "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8";
    try {
        auto socket = IO::Socket::start_socket(1234,50);
        IO::Watcher watcher(socket);
        watcher.Start([&](std::shared_ptr<IO::Socket> sock) {
            try {
                auto req = std::move(Http::HttpUtility::ParseFrom(sock));
                watcher.Stop();
                QVERIFY(req == expected);
            }
            catch (std::exception &ex) {
                std::cout << ex.what() << std::endl;
            }


        });
    }
    catch (std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
    }
}

void Api_testsTest::SiegeOnce()
{
    Http::Request siege;
    siege.method = Http::Request::Method::Get;
    siege.URI = "/";
    siege.version = "HTTP/1.1";
    siege.header.fields["Accept"] = "*/*";
    siege.header.fields["Connection"] = "Keep-Alive";
    siege.header.fields["Host"] = "192.168.2.105:1234";
    siege.header.fields["User-Agent"] = "Wget/1.15 (linux-gnu)";
    try {
        auto socket = IO::Socket::start_socket(1234,50);
        IO::Watcher watcher(socket);
        watcher.Start([&](std::shared_ptr<IO::Socket> sock) {
            try {
                auto req = std::move(Http::HttpUtility::ParseFrom(sock));
                watcher.Stop();
                QVERIFY(req == siege);
            }
            catch (std::exception &ex) {
                std::cout << ex.what() << std::endl;
            }


        });
    }
    catch (std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
    }
}

QTEST_APPLESS_MAIN(Api_testsTest)

#include "tst_api_teststest.moc"
