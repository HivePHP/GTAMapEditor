#pragma once
#include <string>

class GTAData; // Forward declaration

class GtaDatParser {
public:
    // Принимает путь к папке с игрой и объект GTAData для заполнения
    static bool Parse(const std::string& gamePath, GTAData& outData);
};