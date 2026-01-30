/*
 * RDB 持久化测试
 * 测试流程：
 * 1. write_data - 写入测试数据并保存到文件
 * 2. verify_data - 从文件读取数据并验证
 */

#include <QObject>
#include <QtTest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDateTime>
#include <QRandomGenerator>
#include "../fixtures/redistestfixture.h"

class RdbPersistenceTest : public QObject
{
    Q_OBJECT

public:
    RdbPersistenceTest() : fixture_(nullptr) {}

private slots:
    void initTestCase();
    void cleanupTestCase();

    // 测试阶段
    void testWriteData();
    void testVerifyData();

private:
    RedisTestFixture* fixture_;
    QString dataFilePath_;
    
    struct TestData {
        QString key;
        QString value;
    };
    QList<TestData> testData_;
    
    static constexpr int TEST_COUNT = 1000;
    
    void saveTestData();
    bool loadTestData();
};

void RdbPersistenceTest::initTestCase()
{
    fixture_ = new RedisTestFixture();
    QVERIFY2(fixture_->connect(), "Failed to connect to Redis server");
    
    // 从环境变量获取数据文件路径
    dataFilePath_ = QString::fromLocal8Bit(qgetenv("PERSISTENCE_TEST_DATA_FILE"));
    if (dataFilePath_.isEmpty()) {
        dataFilePath_ = "/tmp/rdb_test_data.json";
    }
}

void RdbPersistenceTest::cleanupTestCase()
{
    delete fixture_;
    fixture_ = nullptr;
}

void RdbPersistenceTest::testWriteData()
{
    QVERIFY(fixture_ != nullptr);
    QVERIFY(fixture_->manager() != nullptr);
    
    // 静态变量确保只生成一次数据
    static bool dataGenerated = false;
    static QList<TestData> staticTestData;
    
    if (!dataGenerated) {
        // 生成测试数据（只执行一次）
        for (int i = 0; i < TEST_COUNT; ++i) {
            QString key = QString("rdb_test_key_%1").arg(i);
            QString value = QString("value_%1_%2")
                .arg(QDateTime::currentMSecsSinceEpoch())
                .arg(QRandomGenerator::global()->generate());
            
            TestData data;
            data.key = key;
            data.value = value;
            staticTestData.append(data);
        }
        
        // 复制到成员变量（用于 saveTestData）
        testData_ = staticTestData;
        
        // 保存到文件（只执行一次）
        saveTestData();
        
        dataGenerated = true;
    } else {
        // 后续迭代使用已生成的数据
        testData_ = staticTestData;
    }
    
    // 写入 Redis（每次迭代都执行，用于 benchmark）
    QBENCHMARK {
        for (const auto& data : testData_) {
            QVERIFY(fixture_->manager()->set(data.key, data.value));
        }
    }
    
    qDebug() << "RESULT: Written" << testData_.size() << "keys to Redis";
}

void RdbPersistenceTest::testVerifyData()
{
    QVERIFY(fixture_ != nullptr);
    QVERIFY(fixture_->manager() != nullptr);
    
    // 静态变量确保只执行一次验证逻辑
    static bool verificationDone = false;
    static QJsonObject verificationResult;
    
    if (verificationDone) {
        // 使用之前的结果
        QJsonDocument doc(verificationResult);
        qDebug().noquote() << "RESULT:" << doc.toJson(QJsonDocument::Compact);
        QCOMPARE(verificationResult["mismatched"].toInt(), 0);
        QCOMPARE(verificationResult["matched"].toInt(), TEST_COUNT);
        return;
    }
    

    
    // 从文件加载测试数据
    QVERIFY(loadTestData());
    
    int matched = 0;
    int mismatched = 0;
    QList<QString> failedKeys;
    
    // 验证每个 key（只执行一次验证逻辑）
    for (const auto& data : testData_) {
        QString actualValue = fixture_->manager()->get(data.key);
        
        if (actualValue == data.value) {
            ++matched;
        } else {
            ++mismatched;
            if (failedKeys.size() < 10) {  // 只记录前10个失败的
                failedKeys.append(data.key);
            }
        }
    }
    
    // 保存结果（验证阶段不需要性能测试）
    verificationResult["result"] = (mismatched == 0) ? "PASS" : "FAIL";
    verificationResult["total"] = testData_.size();
    verificationResult["matched"] = matched;
    verificationResult["mismatched"] = mismatched;
    
    QJsonArray failedArray;
    for (const auto& key : failedKeys) {
        failedArray.append(key);
    }
    verificationResult["failed_keys"] = failedArray;
    
    verificationDone = true;
    
    // 输出 JSON 格式的结果
    QJsonDocument doc(verificationResult);
    qDebug().noquote() << "RESULT:" << doc.toJson(QJsonDocument::Compact);
    
    // 验证通过条件
    QCOMPARE(mismatched, 0);
    QCOMPARE(matched, TEST_COUNT);
}

void RdbPersistenceTest::saveTestData()
{
    // 检查文件是否已存在，避免 QBENCHMARK 多次执行时重复追加
    QFile checkFile(dataFilePath_);
    if (checkFile.exists()) {
        qDebug() << "Test data file already exists, skipping save.";
        return;
    }
    
    QJsonArray dataArray;
    for (const auto& data : testData_) {
        QJsonObject obj;
        obj["key"] = data.key;
        obj["value"] = data.value;
        dataArray.append(obj);
    }
    
    QJsonObject root;
    root["count"] = testData_.size();
    root["data"] = dataArray;
    
    QJsonDocument doc(root);
    
    QFile file(dataFilePath_);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write(doc.toJson());
    file.close();
    
    qDebug() << "Test data saved to:" << dataFilePath_ << "(" << testData_.size() << "records )";
}

bool RdbPersistenceTest::loadTestData()
{
    QFile file(dataFilePath_);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open test data file:" << dataFilePath_;
        return false;
    }
    
    QByteArray rawData = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Invalid JSON in test data file";
        return false;
    }
    
    QJsonObject root = doc.object();
    QJsonArray dataArray = root["data"].toArray();
    
    testData_.clear();
    for (const auto& item : dataArray) {
        QJsonObject obj = item.toObject();
        TestData data;
        data.key = obj["key"].toString();
        data.value = obj["value"].toString();
        testData_.append(data);
    }
    
    qDebug() << "Loaded" << testData_.size() << "test records from file";
    return !testData_.isEmpty();
}

QTEST_APPLESS_MAIN(RdbPersistenceTest)
#include "tst_rdbpersistence.moc"
