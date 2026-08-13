#include "GtaDatParser.h"
#include "GTAData.h"
#include "../Core/Logger.h"
#include <fstream>
#include <sstream>
#include <algorithm>

static std::string Trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool GtaDatParser::Parse(const std::string& gamePath, GTAData& outData) {
    outData.m_GamePath = gamePath;
    std::string gtaDatPath = gamePath + "/data/gta.dat";

    Logger::Info("Loading " + gtaDatPath);

    std::ifstream file(gtaDatPath);
    if (!file.is_open()) {
        Logger::Error("Cannot open file: gta.dat at " + gtaDatPath);
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        line = Trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';') continue;

        size_t spacePos = line.find_first_of(" \t");
        if (spacePos == std::string::npos) continue;

        std::string type = line.substr(0, spacePos);
        std::string path = Trim(line.substr(spacePos + 1));

        std::replace(path.begin(), path.end(), '\\', '/');
        std::transform(type.begin(), type.end(), type.begin(), ::toupper);

        if (type == "IDE") {
            outData.m_IdeFiles.push_back(path);
        }
        else if (type == "IPL") {
            outData.m_IplFiles.push_back(path);
        }
        else if (type == "CDIMAGE" || type == "IMG") {
            outData.m_ImgFiles.push_back(path);
        }
    }
    file.close();

    // Ёмул€ци€ поведени€ gta_sa.exe: хардкодим базовые архивы
    outData.m_ImgFiles.insert(outData.m_ImgFiles.begin(), "models/gta_int.img");
    outData.m_ImgFiles.insert(outData.m_ImgFiles.begin(), "models/gta3.img");

    Logger::Info("Found " + std::to_string(outData.m_IdeFiles.size()) + " IDE files");
    Logger::Info("Found " + std::to_string(outData.m_IplFiles.size()) + " IPL files");
    Logger::Info("Found " + std::to_string(outData.m_ImgFiles.size()) + " IMG archives");

    return true;
}