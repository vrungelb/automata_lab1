#include "strgen.h"
#include <algorithm>
#include <fstream>
#include <random>
#include <string>

namespace strgen {
namespace {

constexpr int NUMBER_OF_MAX_EL = 100;
constexpr int NUMBER_OF_NAME   = 10;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> d(0, 1);
constexpr char slovar[] =
    "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";

// случайный символ из slovar
char randChar() {
    return slovar[std::uniform_int_distribution<>(0, sizeof(slovar) - 2)(gen)];
}

// случайное число в диапазоне [a, b]
int randInt(int a, int b) {
    return std::uniform_int_distribution<>(a, b)(gen);
}

// имя: буква, потом буквы и цифры, длина [minLen, maxLen]
std::string genName(int minLen, int maxLen) {
    int len = randInt(minLen, maxLen);
    std::string s;
    s.reserve(len);
    for (int i = 0; i < len; i++) s += randChar();
    return s;
}

std::string genType() {
    static const std::string types[] = {"int", "short", "long"};
    return types[randInt(0, 2)];
}

// литерал: [-][0-9]+
std::string genLiteral() {
    std::string s;
    if (d(gen)) s += '-';
    s += std::to_string(randInt(0, 9999));
    return s;
}

// список из n литералов через запятую
std::string genList(long long n) {
    std::string s;
    if (n <= 0) return s;
    s.reserve(n * 10);
    for (long long i = 0; i < n; i++) {
        if (i > 0) s += ',';
        s += genLiteral();
    }
    return s;
}

// общая реализация для goodStr с заданным размером
std::string makeGood(int count) {
    std::string type = genType();
    std::string name = genName(1, NUMBER_OF_NAME);
    return type + " " + name + "[" + std::to_string(count) + "]={" + genList(count) + "}";
}

// общая реализация для badStr с заданным размером
std::string makeBad(int count) {
    int kind = randInt(0, 16);
    std::string type = genType();
    std::string name = genName(1, 16);
    switch (kind) {
    case 0: {
        static const std::string bad[] = {"float", "double", "char", "INT", "Long"};
        return bad[randInt(0, 4)] + " " + name + "[" + std::to_string(count) + "]={" + genList(count) + "}";
    }
    case 1:
        return type + " " + std::to_string(randInt(0, 9)) + name
               + "[" + std::to_string(count) + "]={" + genList(count) + "}";
    case 2:
        return type + " " + genName(17, 32)
               + "[" + std::to_string(count) + "]={" + genList(count) + "}";
    case 3:
        return type + " " + name + "[0]={" + genList(count) + "}";
    case 4:
        return type + " " + name + "[" + std::to_string(count) + "]={" + genList(count - 1) + "}";
    case 5:
        return type + " " + name + "[" + std::to_string(count) + "]={" + genList(count + 1) + "}";
    case 6:
        return type + " " + name + "[" + std::to_string(count) + "]={--1"
               + (count > 1 ? "," + genList(count - 1) : "") + "}";
    case 7:
        return type + " " + name + "[" + std::to_string(count) + "]=" + genList(count);
    case 8:
        return type + " " + name + "[0" + std::to_string(count) + "]={" + genList(count) + "}";
    case 9:
        return type + " " + name + "[1234567890]={" + genList(count) + "}";
    case 10:
        return type + name + "[" + std::to_string(count) + "]={" + genList(count) + "}";
    case 11:
        return type + " " + name + "[" + std::to_string(count) + "]={" + genList(count) + "}xyz";
    case 12:
        return type + " " + name + "[" + std::to_string(count) + "]={}";
    case 13:
        return type + " " + name + "_x[" + std::to_string(count) + "]={" + genList(count) + "}";
    case 14:
        return type + " " + name + "[" + std::to_string(count) + "]={+1"
               + (count > 1 ? "," + genList(count - 1) : "") + "}";
    case 15:
        return type + " " + name + "[" + std::to_string(count) + "]={1.5"
               + (count > 1 ? "," + genList(count - 1) : "") + "}";
    case 16:
        return type + " " + name + "[" + std::to_string(count) + "]={ " + genList(count) + "}";
    default:
        return makeBad(std::max(1, count));
    }
}

}  // anonymous namespace

std::string goodStr() {
    return makeGood(randInt(1, NUMBER_OF_MAX_EL));
}

std::string badStr() {
    return makeBad(randInt(1, 9));
}

std::string goodStr(int arraySize) {
    return makeGood(arraySize);
}

std::string badStr(int arraySize) {
    return makeBad(arraySize);
}

Stats generateToFile(const std::string& path, int n, int mode) {
    std::ofstream out(path);
    if (!out) return {0, 0};

    int good = 0;
    int bad  = 0;
    for (int i = 0; i < n; i++) {
        bool writeGood;
        if (mode == 1)       writeGood = true;
        else if (mode == 3)  writeGood = false;
        else                 writeGood = (d(gen) == 1);

        if (writeGood) { out << goodStr() << '\n'; ++good; }
        else           { out << badStr()  << '\n'; ++bad;  }
    }
    return {good, bad};
}

Stats generateToFile(const std::string& path, int n, int mode, int arraySize) {
    std::ofstream out(path);
    if (!out) return {0, 0};

    int good = 0;
    int bad  = 0;
    for (int i = 0; i < n; i++) {
        bool writeGood;
        if (mode == 1)       writeGood = true;
        else if (mode == 3)  writeGood = false;
        else                 writeGood = (d(gen) == 1);

        if (writeGood) { out << goodStr(arraySize) << '\n'; ++good; }
        else           { out << badStr(arraySize)  << '\n'; ++bad;  }
    }
    return {good, bad};
}

}  // namespace strgen