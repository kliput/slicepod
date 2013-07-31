#include "precompiled.hpp"

#include <QtCore>
#include <QtTest>

#include <QxtLogger>
#include <taglib/taglib.h>
#include <taglib/fileref.h>

#include "db_model.hpp"
using namespace db::type;

#include "core/utils.hpp"

class SlicepodTest : public QObject
{
	Q_OBJECT
	
public:
	SlicepodTest();

	static const char* DB_NAME;
	
private Q_SLOTS:
	void initTestCase();

	void testSavePodcastWithDir();
	void testReadEpisodes();
	void testGetEpisodeFullPath();
	void testCreateFragments();
	void testTagsCreateAttach();

	void cleanupTestCase();
};

const char* SlicepodTest::DB_NAME = "db.sqlite";

const char* POD1_DIR = "../podcast1";
const char* PODCAST1_NAME = "Some podcast 1";

const char* FRAG1_TITLE = "Chorus";
const char* FRAG1_ARTIST = "Unknown genius";

const char* TAG_GUITARS = "guitars";
const char* TAG_FAST = "fast";
const char* TAG_VOCALS = "vocals";

SlicepodTest::SlicepodTest()
{
}

void SlicepodTest::initTestCase()
{
	qxtLog->enableAllLogLevels();

	qxtLog->info("INIT ----------------------");

	// -- database connection --
	QFile::remove(SlicepodTest::DB_NAME);
	db_connect(SlicepodTest::DB_NAME);
	// -- create all tables in database --
	db_create_tables();
}

void SlicepodTest::cleanupTestCase()
{
}

void SlicepodTest::testSavePodcastWithDir()
{
	// -- save to database

	Podcast::ptr podcast(new Podcast(PODCAST1_NAME));
	QVERIFY2(!qx::dao::save_with_all_relation(podcast).isValid(),
			 "Error saving podcast");

	Directory::ptr directory = scan_dir(POD1_DIR, podcast);
	QVERIFY2(directory, "Null directory pointer");
}

void SlicepodTest::testReadEpisodes()
{
	using namespace db::type;

	// -- verify from database
	{
		using namespace db::field;

		QSqlError error;

		QStringList relations;
		relations << (QStringList()
					  << podcast::EPISODES_LIST
					  << episode::FRAGMENTS_LIST
					  << fragment::EPISODE
					  ).join("->");

		relations << (QStringList()
					  << podcast::EPISODES_LIST
					  << episode::START_FRAGMENT
					  << fragment::EPISODE
					  ).join("->");

		QList<QSharedPointer<Podcast>> result_podcasts_list;
		qx::QxSqlQuery podcast_query;
		podcast_query.where(podcast::NAME).isEqualTo(QString(PODCAST1_NAME));
		error = qx::dao::fetch_by_query_with_relation(relations, podcast_query,
													  result_podcasts_list);
		QVERIFY2(!error.isValid(),
			QString("Fetch podcast failed: %1").arg(error.text()).toAscii());
		QCOMPARE(result_podcasts_list.size(), 1);
		const QSharedPointer<Podcast>& p = result_podcasts_list[0];
		QCOMPARE(p->name, QString(PODCAST1_NAME));
		QCOMPARE(p->episodesList.size(), 4);

		for (auto ep: p->episodesList) {
			QCOMPARE(ep->fragmentsList.size(), 1);
			auto fp = ep->fragmentsList[0];
			QCOMPARE(fp->start, 0);
			QCOMPARE(fp->episode->id, ep->id);
			QCOMPARE(ep->startFragment->id, fp->id);
			QVERIFY2(fp->isStartFragment(), "this fragment should be "
					 "start fragment");
		}
	}
}

void SlicepodTest::testGetEpisodeFullPath()
{
	Episode::ptr_list ep_list;
	qx::dao::fetch_all(ep_list);

	for (auto ep: ep_list) {
		QString fpath = ep->fullPath();
		QVERIFY2(QFile(fpath).exists(), QString("file doesn't exists: %1")
				 .arg(fpath).toAscii());
	}
}


void SlicepodTest::testCreateFragments()
{
	// get first episode of first podcast
	Podcast::ptr_list pod_list;
	QVERIFY2(!qx::dao::fetch_all_with_relation(db::field::podcast::EPISODES_LIST,
								  pod_list).isValid(), "fetch podcasts failed");
	Episode::ptr episode_p = pod_list[0]->episodesList[0];

	int length = episode_p->audioLength();

	{
		Fragment::ptr frag_p(new Fragment);
		frag_p->episode = episode_p;
		frag_p->start = length/3;
		frag_p->end = 2*frag_p->start;
		frag_p->title = str(FRAG1_TITLE);
		frag_p->artist = str(FRAG1_ARTIST);

		QVERIFY2(!qx::dao::save_with_all_relation(frag_p).isValid(), "save fragment"
				 " failed");
	}

	{
		// fetch new fragments from database
		QVERIFY2(!qx::dao::fetch_by_id_with_relation(
					 db::field::episode::FRAGMENTS_LIST,
					 episode_p
					 ).isValid(), "fetch episode with fragments failed");

		const auto& flist = episode_p->fragmentsList;

		QCOMPARE(flist.size(), 2);

		Fragment::ptr_list non_start_fragments;

		// TODO: fragment filters/general entities filters?
		for (const auto& frag_p: flist) {
			if (!frag_p->isStartFragment()) {
				non_start_fragments << frag_p;
			}
		}

		QCOMPARE(non_start_fragments.size(), 1);

	}
}

void SlicepodTest::testTagsCreateAttach()
{
	{
		Tag::ptr tag_g(new Tag(TAG_GUITARS));
		Tag::ptr tag_f(new Tag(TAG_FAST));
		Tag::ptr tag_v(new Tag(TAG_VOCALS));

		QVERIFY(!qx::dao::save(Tag::ptr_list() << tag_g << tag_f << tag_v)
				.isValid());
	}

	long FRAG_ID;

	{
		Tag::ptr_list tags;

		QVERIFY(!qx::dao::fetch_all(tags).isValid());

		QCOMPARE(tags.size(), 3);

		Fragment::ptr_list fragments;
		qx::QxSqlQuery fragment_query;
		fragment_query.where(db::field::fragment::TITLE).isEqualTo(FRAG1_TITLE)
				.and_(db::field::fragment::ARTIST).isEqualTo(FRAG1_ARTIST);

		QVERIFY(!qx::dao::fetch_by_query_with_relation(
					db::field::fragment::TAGS_LIST,	fragment_query, fragments
					).isValid());

		QCOMPARE(fragments.size(), 1);
		Fragment::ptr frag_p = fragments[0];

		FRAG_ID = frag_p->id;

		QCOMPARE(frag_p->tagsList.size(), 0);
		frag_p->tagsList << tags;
		QCOMPARE(frag_p->tagsList.size(), 3);

		QVERIFY(!
			qx::dao::save_with_relation(db::field::fragment::TAGS_LIST, frag_p)
				.isValid());
	}

	{
		Tag::ptr_list tags;

		QVERIFY(!
		  qx::dao::fetch_all_with_relation(db::field::tag::FRAGMENTS_LIST, tags)
				.isValid());

		QCOMPARE(tags.size(), 3);

		for (auto& tag: tags) {
			QCOMPARE(tag->fragmentsList.size(), 1);
			const auto& tag_frag = tag->fragmentsList[0];
			QCOMPARE(tag_frag->id, FRAG_ID);
		}
	}

}


QTEST_APPLESS_MAIN(SlicepodTest)

#include "tst_slicepodtest.moc"
