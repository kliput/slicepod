#ifndef DB_CONSTANTS_HPP
#define DB_CONSTANTS_HPP

#include <QString>
#include <QList>
#include <QSharedPointer>

namespace db {

namespace field {

// TODO: maybe change all constexpr const char* to const QString?

constexpr const char* ID = "id";

namespace directory {
constexpr const char* PATH = "path";
constexpr const char* METADATA = "metadata";
constexpr const char* EPISODES_LIST = "episodes_list";
}

namespace episode {
constexpr const char* FILE_NAME = "file_name";
constexpr const char* EPISODE_NAME = "episode_name";
constexpr const char* DIRECTORY = "directory_id";
constexpr const char* PODCAST = "podcast_id";
constexpr const char* START_FRAGMENT = "start_fragment_id";
constexpr const char* COMPARE_DATA = "compare_data";
constexpr const char* METADATA = "metadata";
constexpr const char* FRAGMENTS_LIST = "fragments_list";
}

namespace fragment {
constexpr const char* EPISODE = "episode_id";
constexpr const char* START = "start";
constexpr const char* END = "end";
constexpr const char* TITLE = "title";
constexpr const char* ARTIST = "artist";
constexpr const char* METADATA = "metadata";
constexpr const char* TAGS_LIST = "tags_list";
constexpr const char* PLAYLISTS_LIST = "playlists_list";
}

namespace playlist {
constexpr const char* NAME = "name";
constexpr const char* METADATA = "metadata";
constexpr const char* FRAGMENTS_LIST = "fragments_list";
}

namespace podcast {
constexpr const char* NAME = "name";
constexpr const char* METADATA = "metadata";
constexpr const char* EPISODES_LIST = "episodes_list";
}

namespace tag {
constexpr const char* NAME = "name";
constexpr const char* METADATA = "metadata";
constexpr const char* FRAGMENTS_LIST = "fragments_list";
}

namespace fragment_tag_map {
constexpr const char* TABLE_NAME = "FragmentTagMap";
constexpr const char* TAG = "tag_id";
constexpr const char* FRAGMENT = "fragment_id";
}

namespace playlist_fragment_map {
constexpr const char* TABLE_NAME = "PlaylistFragmentMap";
constexpr const char* PLAYLIST = "playlist_id";
constexpr const char* FRAGMENT = "fragment_id";
}

}

// TODO: namespace doesn't work! - change to struct or something...
namespace type {

using str = QString;
template <class T> using lst = QList<T>;
template <class T> using ptr = QSharedPointer<T>;
template <class T> using ptr_list = lst<ptr<T>>;

}

}

#endif // DB_CONSTANTS_HPP
