#include <QtCore>
#include <QtTest>

#include "db_engine/databaseengine.hpp"
#include "db_engine/podcast.hpp"
#include "db_engine/directory.hpp"
#include "db_engine/episode.hpp"

const QString dbPath = "test_db.sqlite3";

class TestsTest : public QObject
{
	Q_OBJECT
	
public:
	TestsTest();
	
private Q_SLOTS:
	void testEngineOpen();
	void testInsertSingle();
	void testInsertMultipleEpisodes();

private:
	DatabaseEngine* db = DatabaseEngine::getInstance();
};

TestsTest::TestsTest()
{
}

void TestsTest::testEngineOpen()
{
	QFile(dbPath).remove();
	db->open(dbPath);
	db->createAllTables();
	QVERIFY2(db->isOpened(), "DatabaseEngine not opened");
}

void TestsTest::testInsertSingle()
{
	QCOMPARE(Directory("directory1").save()->getPath(), QString("directory1"));
	QCOMPARE(Podcast("podcast1").save()->getName(), QString("podcast1"));
}

void TestsTest::testInsertMultipleEpisodes()
{
	const int count = 10;

	Directory::ptr dir = db->list<Directory>()[0];
	Podcast::ptr pod = db->list<Podcast>()[0];

	QVERIFY(dir);
	QVERIFY(pod);

	QList<Episode> episodes;
	for (int i=0; i<count; ++i) {
		episodes << Episode(QString("ep-%1").arg(i), QString("Episode #%1").arg(i),
							pod, dir);
	}
	QCOMPARE(episodes.size(), count);

	int preInsertCount = db->list<Episode>().size();

	QCOMPARE(db->insertMultiple<Episode>(episodes).size(), count);

	QCOMPARE(db->list<Episode>().size(), preInsertCount + count);
}

QTEST_APPLESS_MAIN(TestsTest)

#include "tst_teststest.moc"
