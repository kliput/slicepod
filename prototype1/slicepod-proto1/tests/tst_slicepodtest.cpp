#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QtTest>

#include <QxtLogger>

#include "precompiled.hpp"
#include "db_model.hpp"

#include <core/utils.hpp>

class SlicepodTest : public QObject
{
	Q_OBJECT
	
public:
	SlicepodTest();

	static const char* DB_NAME;
	
private Q_SLOTS:
	void initTestCase();
	void cleanupTestCase();
	void testCreateEpisode();
};

const char* SlicepodTest::DB_NAME = "db.sqlite";

SlicepodTest::SlicepodTest()
{
}

void SlicepodTest::initTestCase()
{
	QSqlError err;
	qxtLog->enableAllLogLevels();

	// -- database connection --
	QFile::remove(SlicepodTest::DB_NAME);
	db_connect(SlicepodTest::DB_NAME);
	// -- create all tables in database --
	err = qx::dao::create_table<Episode>();
	err = qx::dao::create_table<Directory>();
	err = qx::dao::create_table<Podcast>();
	err = qx::dao::create_table<Fragment>();
	err = qx::dao::create_table<Tag>();
	err = qx::dao::create_table<Playlist>();
}

void SlicepodTest::cleanupTestCase()
{
}

void SlicepodTest::testCreateEpisode()
{
	using namespace db::type;

	const char* POD1_DIR = "../podcast1";
	const char* PODCAST1_NAME = "Some podcast 1";

	// -- save to database
	{
		ptr<Podcast> podcast(new Podcast(PODCAST1_NAME));
		QVERIFY2(!qx::dao::save_with_all_relation(podcast).isValid(),
				 "Error saving podcast");

		ptr<Directory> directory = scan_dir(POD1_DIR, podcast);
		QVERIFY2(directory, "Null directory pointer");
	}


	// -- verify from database
	{
		using namespace db::field;

		QSqlError error;

		// TODO: wait for bugfix in QxOrm 1.2.6 beta 3
//		QStringList relations;
//		relations << (QStringList()
//					  << podcast::EPISODES_LIST
//					  << episode::FRAGMENTS_LIST
//					  << fragment::EPISODE
//					  ).join("->");

//		relations << (QStringList()
//					  << podcast::EPISODES_LIST
//					  << episode::START_FRAGMENT
//					  << fragment::EPISODE
//					  ).join("->");

		QList<QSharedPointer<Podcast>> result_podcasts_list;
		qx::QxSqlQuery podcast_query;
		podcast_query.where(podcast::NAME).isEqualTo(QString(PODCAST1_NAME));
		// TODO: check and report bug to QxOrm website
//		error = qx::dao::fetch_by_query_with_relation("*", podcast_query,
//													  result_podcasts_list);
		// TODO: "relation" argument should be "relations" variable
		error = qx::dao::fetch_all_with_relation("*->*->*", result_podcasts_list);
		QVERIFY2(!error.isValid(),
			QString("Fetch podcast failed: %1").arg(error.text()).toAscii());
		QCOMPARE(result_podcasts_list.size(), 1);
		const QSharedPointer<Podcast>& p = result_podcasts_list[0];
		QCOMPARE(p->name, QString(PODCAST1_NAME));
		QCOMPARE(p->episodes_list.size(), 4);


		for (auto ep: p->episodes_list) {
			QCOMPARE(ep->fragments_list.size(), 1);
			auto fp = ep->fragments_list[0];
			QCOMPARE(fp->start, 0);
			QCOMPARE(fp->episode->id, ep->id);
			QCOMPARE(ep->start_fragment->id, fp->id);
			QVERIFY2(fp->is_start_fragment(), "this fragment should be "
					 "start fragment");
		}
	}
}

QTEST_APPLESS_MAIN(SlicepodTest)

#include "tst_slicepodtest.moc"
