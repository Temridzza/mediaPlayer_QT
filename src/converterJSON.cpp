#include "converterJSON.h"


#include <fstream>
#include <QFile>
#include <fstream>
#include <QDebug>

#include <iostream>

void ConverterJSON::expFile(const std::string &nameDir, const std::string &nameAudio, const std::string &progress) {
    using json = nlohmann::json;
    // Путь к файлу JSON
    std::string filePath = "./history.json";

    // Загрузка существующего файла JSON или создание нового
    nlohmann::json j;
    try {
        std::ifstream file(filePath);
        if (file.is_open()) {
            file >> j;
            file.close();
        }
    } catch (...) {
        // Если файл не существует, создаем новый пустой объект
        j = nlohmann::json::object();
    }

    // Проверяем наличие папки в JSON
    if (!j.contains(nameDir)) {
        // Если папка не существует, создаем новую
        j[nameDir] = nlohmann::json::array();
    }

    // Добавляем новую запись к существующей папке
    j[nameDir].push_back({
        {"nameAudio", nameAudio},
        {"progress", progress}
    });

    // Записываем обновленный JSON обратно в файл
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << j.dump(4);  // форматирование с отступами
        outFile.close();
    } else {
        qDebug() << "Error when writing to a file: ";
    }
}
