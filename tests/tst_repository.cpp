#include "tst_repository.h"
#include "testutils.h"
#include <repository.h>
#include <QString>

const QString testDir = QString(SRCDIR);
const QString testNew = testDir + "repository_testNew";
const QString testAdd = testDir + "repository_testAdd";

void TestRepository::initTestCase()
{
    FORCED_CLEANUP
    if(!QFile::copy(testAdd + "/init/versions", testAdd + "/dir/versions"))
        QFAIL("Unable to copy init/versions to dir/versions");
    if(!QFile::copy(testAdd + "/init/packages", testAdd + "/dir/packages"))
        QFAIL("Unable to copy init/packages to dir/packages");
    if(!QFile::copy(testAdd + "/init/current", testAdd + "/dir/current"))
        QFAIL("Unable to copy init/current to dir/current");
}

void TestRepository::cleanupTestCase()
{
    if(!TestUtils::cleanup)
        return;

    QFile::remove(testAdd + "/dir/versions");
    QFile::remove(testAdd + "/dir/packages");
    QFile::remove(testAdd + "/dir/current");
}

void TestRepository::testNewRepository()
{
    QString dir = testNew + "\\dir";
    Repository pm;

    QCOMPARE(pm.directory(), QString());

    pm.setDirectory(dir);
    QCOMPARE(pm.directory(), testNew + "/dir/");

    try {
        pm.load();
        QCOMPARE(pm.packages().size(), 0);
        QCOMPARE(pm.versions().size(), 0);
        QVERIFY(pm.currentRevision().isEmpty());
    } catch(QString & msg) {
        QFAIL(("Load failed : " + msg).toLatin1());
    }

    try {
        pm.save();
        QVERIFY(!QFile::exists(testNew + "/expected/current"));
        QVERIFY(TestUtils::compareJson(testNew + "/dir/packages", testNew + "/expected/packages"));
        QVERIFY(TestUtils::compareJson(testNew + "/dir/versions", testNew + "/expected/versions"));
    } catch(QString & msg) {
        QFAIL(("Save failed : " + msg).toLatin1());
    }

    try {
        Repository pm2;
        pm2.setDirectory(testNew + "/dir");
        pm2.load();
        QCOMPARE(pm2.packages().size(), 0);
        QCOMPARE(pm2.versions().size(), 0);
        QVERIFY(pm2.currentRevision().isEmpty());
    } catch(QString & msg) {
        QFAIL(("2nd load failed : " + msg).toLatin1());
    }
}

void TestRepository::testAddPackage()
{
    Repository pm;
    pm.setDirectory(testAdd + "/dir");
    QCOMPARE(pm.directory(), testAdd + "/dir/");

    try {
        pm.load();
        QCOMPARE(pm.packages().size(), 1);
        QCOMPARE(pm.versions().size(), 1);
        QCOMPARE(pm.currentRevision(), QString("REV1"));
    } catch(QString & msg) {
        QFAIL(("Load failed : " + msg).toLatin1());
    }

    pm.addPackage("test2");
    QCOMPARE(pm.packages().size(), 2);
    QCOMPARE(pm.versions().size(), 2);
    QCOMPARE(pm.currentRevision(), QString("REV1"));

    QCOMPARE(pm.setCurrentRevision(QString("REV2")), true);
    QCOMPARE(pm.currentRevision(), QString("REV2"));

    QCOMPARE(pm.setCurrentRevision(QString("REV3")), false);
    QCOMPARE(pm.currentRevision(), QString("REV2"));

    try {
        pm.save();
        QVERIFY(TestUtils::compareJson(testAdd + "/dir/current", testAdd + "/expected/current"));
        QVERIFY(TestUtils::compareJson(testAdd + "/dir/packages", testAdd + "/expected/packages"));
        QVERIFY(TestUtils::compareJson(testAdd + "/dir/versions", testAdd + "/expected/versions"));
    } catch(QString & msg) {
        QFAIL(("Save failed : " + msg).toLatin1());
    }

    try {
        Repository pm2;
        pm2.setDirectory(testAdd + "/dir");
        pm2.load();
        QCOMPARE(pm2.packages().size(), 2);
        QCOMPARE(pm2.versions().size(), 2);
        QCOMPARE(pm2.currentRevision(), QString("REV2"));
    } catch(QString & msg) {
        QFAIL(("2nd load failed : " + msg).toLatin1());
    }
}

void TestRepository::testFixRepository()
{
    QWARN("test not implemented");
}
