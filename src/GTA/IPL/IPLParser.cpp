#include "IPLParser.h"
#include "../../Core/Logger.h"
#include <fstream>
#include <sstream>
#include <algorithm>

static std::string Trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

static std::vector<std::string> Split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

IPLParser::IPLParser() {}
IPLParser::~IPLParser() {}

bool IPLParser::Load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        // Обязательно выводим полное название с расширением!
        Logger::Error("Cannot open file: " + filepath);
        return false;
    }

    std::string line;
    std::string currentSection = "";

    while (std::getline(file, line)) {
        line = Trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';') continue;

        std::string lowerLine = line;
        std::transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);

        if (lowerLine == "end") {
            currentSection = "";
            continue;
        }

        if (lowerLine.find(',') == std::string::npos && lowerLine.length() <= 5) {
            currentSection = lowerLine;
            continue;
        }

        // Парсим секцию расстановки объектов (inst)
        if (currentSection == "inst") {
            std::vector<std::string> tokens = Split(line, ',');

            // В формате San Andreas обычно 11 параметров в строке inst
            if (tokens.size() >= 11) {
                IPLPlacement placement;
                try {
                    placement.id = std::stoi(Trim(tokens[0]));
                    placement.modelName = Trim(tokens[1]);
                    placement.interior = std::stoi(Trim(tokens[2]));

                    placement.posX = std::stof(Trim(tokens[3]));
                    placement.posY = std::stof(Trim(tokens[4]));
                    placement.posZ = std::stof(Trim(tokens[5]));

                    placement.rotX = std::stof(Trim(tokens[6]));
                    placement.rotY = std::stof(Trim(tokens[7]));
                    placement.rotZ = std::stof(Trim(tokens[8]));
                    placement.rotW = std::stof(Trim(tokens[9]));

                    placement.lod = std::stoi(Trim(tokens[10]));

                    m_Placements.push_back(placement);
                }
                catch (...) {
                    // Игнорируем мусорные строки
                }
            }
        }
    }

    file.close();
    return true;
}