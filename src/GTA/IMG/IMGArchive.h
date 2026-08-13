#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

class IMGArchive {
public:
    IMGArchive();
    ~IMGArchive();

    bool Open(const std::string& path);
    bool Contains(const std::string& filename) const;
    bool ReadFile(const std::string& filename, std::vector<uint8_t>& data) const;

private:
    struct Entry {
        uint32_t offsetSectors;
        uint32_t sizeSectors;
    };

    std::string m_ArchivePath;
    std::unordered_map<std::string, Entry> m_Entries;
};