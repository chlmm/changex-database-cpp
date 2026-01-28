#include <QObject>
#include <QtTest>
#include <QRandomGenerator>
#include "../fixtures/redistestfixture.h"

class StringBenchmark : public QObject
{
    Q_OBJECT

public:
    StringBenchmark() : fixture_(nullptr) {}

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // String CRUD Benchmarks
    void benchmarkStringCreate();
    void benchmarkStringRead();
    void benchmarkStringUpdate();
    void benchmarkStringDelete();

private:
    RedisTestFixture *fixture_;
    QString testKey_;
    QString testValue_;
};

void StringBenchmark::initTestCase()
{
    fixture_ = new RedisTestFixture();
    QVERIFY2(fixture_->connect(), "Failed to connect to Redis server");
}

void StringBenchmark::cleanupTestCase()
{
    delete fixture_;
    fixture_ = nullptr;
}

void StringBenchmark::init()
{
    testKey_ = RedisTestFixture::generateUniqueKey("str");
    testValue_ = "benchmark_test_value_" + QString::number(QRandomGenerator::global()->generate());
}

void StringBenchmark::cleanup()
{
    if (fixture_ && fixture_->manager()) {
        fixture_->manager()->del(testKey_);
    }
}

// Create: SET操作性能测试
void StringBenchmark::benchmarkStringCreate()
{
    QBENCHMARK {
        fixture_->manager()->set(testKey_, testValue_);
    }
}

// Read: GET操作性能测试
void StringBenchmark::benchmarkStringRead()
{
    fixture_->manager()->set(testKey_, testValue_);
    QBENCHMARK {
        fixture_->manager()->get(testKey_);
    }
}

// Update: SET覆盖操作性能测试
void StringBenchmark::benchmarkStringUpdate()
{
    fixture_->manager()->set(testKey_, testValue_);
    QString newValue = "updated_value_" + QString::number(QRandomGenerator::global()->generate());
    QBENCHMARK {
        fixture_->manager()->set(testKey_, newValue);
    }
}

// Delete: DEL操作性能测试
void StringBenchmark::benchmarkStringDelete()
{
    QBENCHMARK {
        fixture_->manager()->set(testKey_, testValue_);
        fixture_->manager()->del(testKey_);
    }
}

QTEST_APPLESS_MAIN(StringBenchmark)
#include "tst_stringbenchmark.moc"
