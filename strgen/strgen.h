#pragma once
#include <string>

namespace strgen {

    // существующие функции, размер массива случайный
    std::string goodStr();
    std::string badStr();

    // новые перегрузки для бенчмарка, размер массива задан
    std::string goodStr(int arraySize);
    std::string badStr(int arraySize);

    struct Stats {
        int good;
        int bad;
    };

    // mode: 1 - только валидные, 2 - смешанные, 3 - только невалидные
    Stats generateToFile(const std::string& path, int n, int mode);

    // версия с фиксированным размером массива во всех строках
    Stats generateToFile(const std::string& path, int n, int mode, int arraySize);

}  // namespace strgen