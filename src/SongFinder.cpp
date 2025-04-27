#include <DanceDance/SongFinder.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace {

std::string getTitle(const std::filesystem::path& path) {
    if (!exists(path)) {
        return {};
    }

    std::ifstream stream(path);
    if (!stream.is_open()) {
        return {};
    }

    std::string title;
    std::getline(stream, title);
    return title;
}

} // namespace

namespace dd {

SongFinder& SongFinder::GetSongFinder() {
    static SongFinder songFinder(std::filesystem::absolute("assets"));
    return songFinder;
}

SongFinder::SongFinder(const std::filesystem::path& assetsPath) {
    namespace fs = std::filesystem;
    const auto tapePath = assetsPath / "tapes";
    const auto audioPath = assetsPath / "music";
    if (!fs::exists(tapePath) || !fs::exists(audioPath) || !fs::exists(assetsPath)) {
        return;
    }

    std::ranges::for_each(fs::directory_iterator(tapePath), [this, &audioPath](const auto& tapeEntry) {
        if (!tapeEntry.is_regular_file() || tapeEntry.path().extension() != ".tape") {
            return;
        }
        std::ranges::for_each(fs::directory_iterator(audioPath), [this, &tapeEntry](const auto& audioEntry) {
            if (tapeEntry.path().stem() == audioEntry.path().stem()) {
                m_foundSongs.emplace_back(Song{
                    .title = getTitle(tapeEntry),
                    .audioPath = audioEntry,
                    .tapePath = tapeEntry,
                });
            }
        });
    });
}

const std::vector<SongFinder::Song>& SongFinder::getFoundSongs() const { return m_foundSongs; }

} // namespace dd