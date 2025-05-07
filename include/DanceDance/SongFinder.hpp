#ifndef DANCE_DANCE_SONG_FINDER_HPP
#define DANCE_DANCE_SONG_FINDER_HPP

#include <filesystem>
#include <string>
#include <vector>

namespace dd {

class SongFinder {
  public:
    struct Song {
        std::string title;
        std::filesystem::path audioPath;
        std::filesystem::path tapePath;
    };

    static SongFinder& GetSongFinder();
    const std::vector<Song>& getFoundSongs() const;

  private:
    explicit SongFinder(const std::filesystem::path& assetsPath);
    std::vector<Song> m_foundSongs;
};

} // namespace dd

#endif // DANCE_DANCE_SONG_FINDER_HPP