#pragma once
#include <string>
#include <vector>

class GTAData {
public:
    std::string m_GamePath;

    std::vector<std::string> m_IdeFiles;
    std::vector<std::string> m_IplFiles;
    std::vector<std::string> m_ImgFiles;

    GTAData();
    ~GTAData();
};