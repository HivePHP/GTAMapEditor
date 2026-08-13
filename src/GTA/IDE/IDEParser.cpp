#include "IDEParser.h"
#include "../../Core/Logger.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

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

IDEParser::IDEParser() {}
IDEParser::~IDEParser() {}

bool IDEParser::Load(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::Error("Cannot open file: " + filepath);
        return false;
    }

    std::string line;
    std::string currentSection = "";

    while (std::getline(file, line)) {
        line = Trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }

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

        if (currentSection == "objs" || currentSection == "tobj") {
            std::vector<std::string> tokens = Split(line, ',');

            if (tokens.size() >= 3) {
                IDEObject obj;
                try {
                    obj.id = std::stoi(Trim(tokens[0]));
                    obj.modelName = Trim(tokens[1]);
                    obj.txdName = Trim(tokens[2]);

                    std::transform(obj.modelName.begin(), obj.modelName.end(), obj.modelName.begin(), ::tolower);
                    std::transform(obj.txdName.begin(), obj.txdName.end(), obj.txdName.begin(), ::tolower);

                    m_Objects[obj.id] = obj;
                    m_ObjectsList.push_back(obj); // Сохраняем для индексации в UI
                }
                catch (...) {
                    // Игнорируем ошибки конвертации
                }
            }
        }
    }

    file.close();
    return true;
}

const IDEObject* IDEParser::Find(int id) const {
    auto it = m_Objects.find(id);
    if (it != m_Objects.end()) {
        return &(it->second);
    }
    return nullptr;
}