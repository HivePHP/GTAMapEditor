#include "Logger.h"
#include <windows.h>
#include <iostream>

void Logger::Init() {
    // Создаем консоль для вывода текста
    AllocConsole();

    // Перенаправляем стандартный вывод в эту консоль
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);

    Info("Logger initialized");
}

void Logger::Info(const std::string& message) {
    std::cout << "[INFO] " << message << "\n";
}

void Logger::Error(const std::string& message) {
    // Обязательно выводим полное сообщение, где будет указано название файла с расширением
    std::cout << "[ERROR] " << message << "\n";
}