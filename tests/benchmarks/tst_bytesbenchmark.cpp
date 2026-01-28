#include <QObject>
#include <QtTest>
#include <QRandomGenerator>
#include "../fixtures/redistestfixture.h"

class BytesBenchmark : public QObject
{
    Q_OBJECT

public:
    BytesBenchmark() : fixture_(nullptr) {}

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Bytes CRUD Benchmarks - 不同大小
    void benchmarkBytesCreate1KB();
    void benchmarkBytesCreate10KB();
    void benchmarkBytesCreate100KB();
    void benchmarkBytesCreate1MB();

    void benchmarkBytesRead1KB();
    void benchmarkBytesRead100KB();
    void benchmarkBytesRead1MB();

    void benchmarkBytesUpdate1KB();

    void benchmarkBytesDelete();

    void benchmarkBytesAppend1KB();

private:
    RedisTestFixture *fixture_;
    QString testKey_;
    QByteArray generateRandomBytes(int size);
};

void BytesBenchmark::initTestCase()
{
    fixture_ = new RedisTestFixture();
    QVERIFY2(fixture_->connect(), "Failed to connect to Redis server");
}

void BytesBenchmark::cleanupTestCase()
{
    delete fixture_;
    fixture_ = nullptr;
}

void BytesBenchmark::init()
{
    testKey_ = RedisTestFixture::generateUniqueKey("bytes");
}

void BytesBenchmark::cleanup()
{
    if (fixture_ && fixture_->manager()) {
        fixture_->manager()->bytesDel(testKey_);
    }
}

QByteArray BytesBenchmark::generateRandomBytes(int size)
{
    QByteArray data;
    data.reserve(size);
    for (int i = 0; i < size; ++i) {
        data.append(static_cast<char>(QRandomGenerator::global()->generate() % 256));
    }
    return data;
}

// Create: 1KB字节流写入
void BytesBenchmark::benchmarkBytesCreate1KB()
{
    QByteArray data = generateRandomBytes(1024);
    QBENCHMARK {
        fixture_->manager()->bytesSet(testKey_, data);
    }
}

// Create: 10KB字节流写入
void BytesBenchmark::benchmarkBytesCreate10KB()
{
    QByteArray data = generateRandomBytes(10 * 1024);
    QBENCHMARK {
        fixture_->manager()->bytesSet(testKey_, data);
    }
}

// Create: 100KB字节流写入
void BytesBenchmark::benchmarkBytesCreate100KB()
{
    QByteArray data = generateRandomBytes(100 * 1024);
    QBENCHMARK {
        fixture_->manager()->bytesSet(testKey_, data);
    }
}

// Create: 1MB字节流写入
void BytesBenchmark::benchmarkBytesCreate1MB()
{
    QByteArray data = generateRandomBytes(1024 * 1024);
    QBENCHMARK {
        fixture_->manager()->bytesSet(testKey_, data);
    }
}

// Read: 1KB字节流读取
void BytesBenchmark::benchmarkBytesRead1KB()
{
    QByteArray data = generateRandomBytes(1024);
    fixture_->manager()->bytesSet(testKey_, data);
    QBENCHMARK {
        fixture_->manager()->bytesGet(testKey_);
    }
}

// Read: 100KB字节流读取
void BytesBenchmark::benchmarkBytesRead100KB()
{
    QByteArray data = generateRandomBytes(100 * 1024);
    fixture_->manager()->bytesSet(testKey_, data);
    QBENCHMARK {
        fixture_->manager()->bytesGet(testKey_);
    }
}

// Read: 1MB字节流读取
void BytesBenchmark::benchmarkBytesRead1MB()
{
    QByteArray data = generateRandomBytes(1024 * 1024);
    fixture_->manager()->bytesSet(testKey_, data);
    QBENCHMARK {
        fixture_->manager()->bytesGet(testKey_);
    }
}

// Update: 1KB字节流覆盖
void BytesBenchmark::benchmarkBytesUpdate1KB()
{
    QByteArray data = generateRandomBytes(1024);
    fixture_->manager()->bytesSet(testKey_, data);
    QByteArray newData = generateRandomBytes(1024);
    QBENCHMARK {
        fixture_->manager()->bytesSet(testKey_, newData);
    }
}

// Delete: 字节流删除
void BytesBenchmark::benchmarkBytesDelete()
{
    QByteArray data = generateRandomBytes(1024);
    QBENCHMARK {
        fixture_->manager()->bytesSet(testKey_, data);
        fixture_->manager()->bytesDel(testKey_);
    }
}

// Append: 1KB字节流追加
void BytesBenchmark::benchmarkBytesAppend1KB()
{
    QByteArray data = generateRandomBytes(1024);
    fixture_->manager()->bytesSet(testKey_, data);
    QByteArray appendData = generateRandomBytes(1024);
    QBENCHMARK {
        fixture_->manager()->bytesAppend(testKey_, appendData);
    }
}

QTEST_APPLESS_MAIN(BytesBenchmark)
#include "tst_bytesbenchmark.moc"
