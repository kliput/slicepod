#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QtTest>

#include <QxtLogger>

#include "precompiled.hpp"
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
	void cleanupTestCase();
	void testSavePodcastWithDir();
	void testReadEpisodes();
	void testGetEpisodeFullPath();
	void testCreateFragments();
	void testTagsCreateAttach();
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
	QSqlError err;
	qxtLog->enableAllLogLevels();

	qxtLog->info("INIT ----------------------");

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

void SlicepodTest::testSavePodcastWithDir()
{
	// -- save to database

	ptr<Podcast> podcast(new Podcast(PODCAST1_NAME));
	QVERIFY2(!qx::dao::save_with_all_relation(podcast).isValid(),
			 "Error saving podcast");

	ptr<Directory> directory = scan_dir(POD1_DIR, podcast);
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

void SlicepodTest::testGetEpisodeFullPath()
{
	db::type::ptr_list<Episode> ep_list;
	qx::dao::fetch_all(ep_list);

	for (auto ep: ep_list) {
		QString fpath = ep->full_path();
		QVERIFY2(QFile(fpath).exists(), QString("file doesn't exists: %1")
				 .arg(fpath).toAscii());
	}
}


void SlicepodTest::testCreateFragments()
{
	// get first episode of first podcast
	db::type::ptr_list<Podcast> pod_list;
	QVERIFY2(!qx::dao::fetch_all_with_relation(db::field::podcast::EPISODES_LIST,
								  pod_list).isValid(), "fetch podcasts failed");
	db::type::ptr<Episode> episode_p = pod_list[0]->episodes_list[0];

	int length = episode_p->audio_length();

	{
		ptr<Fragment> frag_p(new Fragment);
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

		const auto& flist = episode_p->fragments_list;

		QCOMPARE(flist.size(), 2);

		db::type::ptr_list<Fragment> non_start_fragments;

		// TODO: fragment filters/general entities filters?
		for (const auto& frag_p: flist) {
			if (!frag_p->is_start_fragment()) {
				non_start_fragments << frag_p;
			}
		}

		QCOMPARE(non_start_fragments.size(), 1);

	}
}

void SlicepodTest::testTagsCreateAttach()
{
	{
		ptr<Tag> tag_g(new Tag(TAG_GUITARS));
		ptr<Tag> tag_f(new Tag(TAG_FAST));
		ptr<Tag> tag_v(new Tag(TAG_VOCALS));

		QVERIFY(!qx::dao::save(ptr_list<Tag>() << tag_g << tag_f << tag_v)
				.isValid());
	}

	long FRAG_ID;

	{
		ptr_list<Tag> tags;

		QVERIFY(!qx::dao::fetch_all(tags).isValid());

		QCOMPARE(tags.size(), 3);

		ptr_list<Fragment> fragments;
		qx::QxSqlQuery fragment_query;
		fragment_query.where(db::field::fragment::TITLE).isEqualTo(FRAG1_TITLE)
				.and_(db::field::fragment::ARTIST).isEqualTo(FRAG1_ARTIST);

		QVERIFY(!qx::dao::fetch_by_query_with_relation(
					db::field::fragment::TAGS_LIST,	fragment_query, fragments
					).isValid());

		QCOMPARE(fragments.size(), 1);
		ptr<Fragment> frag_p = fragments[0];

		FRAG_ID = frag_p->id;

		QCOMPARE(frag_p->tags_list.size(), 0);
		frag_p->tags_list << tags;
		QCOMPARE(frag_p->tags_list.size(), 3);

		QVERIFY(!
			qx::dao::save_with_relation(db::field::fragment::TAGS_LIST, frag_p)
				.isValid());
	}

	{
		ptr_list<Tag> tags;

		QVERIFY(!
		  qx::dao::fetch_all_with_relation(db::field::tag::FRAGMENTS_LIST, tags)
				.isValid());

		QCOMPARE(tags.size(), 3);

		for (auto& tag: tags) {
			QCOMPARE(tag->fragments_list.size(), 1);
			const auto& tag_frag = tag->fragments_list[0];
			QCOMPARE(tag_frag->id, FRAG_ID);
		}
	}

}


QTEST_APPLESS_MAIN(SlicepodTest)

#include "tst_slicepodtest.moc"
