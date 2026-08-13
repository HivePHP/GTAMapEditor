#include "IMGArchive.h"
#include "../../Core/Logger.h"
#include <fstream>
#include <algorithm>
#include <cstring>

static std::string ToLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Структура одного файла внутри IMG VER2 (всегда 32 байта)
#pragma pack(push, 1)
struct VER2DirEntry {
    uint32_t offset;
    uint16_t streamingSize;
    uint16_t sizeInArchive;
    char name[24];
};
#pragma pack(pop)

IMGArchive::IMGArchive() {}
IMGArchive::~IMGArchive() {}

bool IMGArchive::Open(const std::string& path) {
    m_ArchivePath = path;
    m_Entries.clear();

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        Logger::Error("Cannot open file: gta3.img at " + path);
        return false;
    }

    char magic[4];
    file.read(magic, 4);
    if (strncmp(magic, "VER2", 4) != 0) {
        Logger::Error("Cannot open file: " + path + " (Unsupported IMG format, expected VER2)");
        return false;
    }

    uint32_t entryCount = 0;
    file.read(reinterpret_cast<char*>(&entryCount), 4);

    std::vector<VER2DirEntry> entries(entryCount);
    file.read(reinterpret_cast<char*>(entries.data()), entryCount * sizeof(VER2DirEntry));

    for (uint32_t i = 0; i < entryCount; ++i) {
        // Очищаем мусор из имени, гарантируя null-терминатор
        char cleanName[25] = { 0 };
        memcpy(cleanName, entries[i].name, 24);
        std::string nameStr(cleanName);

        Entry e;
        e.offsetSectors = entries[i].offset;
        e.sizeSectors = entries[i].streamingSize != 0 ? entries[i].streamingSize : entries[i].sizeInArchive;

        // Ключи сохраняем в нижнем регистре для быстрого и нечувствительного к регистру поиска
        m_Entries[ToLower(nameStr)] = e;
    }

    Logger::Info("Opening " + path);
    Logger::Info("IMG entries: " + std::to_string(entryCount));
    return true;
}

bool IMGArchive::Contains(const std::string& filename) const {
    return m_Entries.find(ToLower(filename)) != m_Entries.end();
}

bool IMGArchive::ReadFile(const std::string& filename, std::vector<uint8_t>& data) const {
    auto it = m_Entries.find(ToLower(filename));
    if (it == m_Entries.end()) {
        Logger::Error("Cannot open file: " + filename + " (Not found in IMG archive)");
        return false;
    }

    std::ifstream file(m_ArchivePath, std::ios::binary);
    if (!file.is_open()) {
        Logger::Error("Cannot open file: gta3.img at " + m_ArchivePath);
        return false;
    }

    const Entry& e = it->second;

    // В IMG смещения и размеры хранятся в секторах. 1 сектор = 2048 байт.
    uint32_t byteOffset = e.offsetSectors * 2048;
    uint32_t byteSize = e.sizeSectors * 2048;

    data.resize(byteSize);
    file.seekg(byteOffset, std::ios::beg);
    file.read(reinterpret_cast<char*>(data.data()), byteSize);

    return true;
}