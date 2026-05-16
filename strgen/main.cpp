#include <algorithm>
#include <iostream>
#include <random>
#include <fstream>
#include <string>
#define NUMBER_OF_STRINGS 100
#define NUMBER_OF_MAX_EL 100
#define NUMBER_OF_NAME 10
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> d(0, 1);
static constexpr char slovar[] = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";
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
    for (int i = 0; i < len; i++)
        s += randChar();
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
    s.reserve(n * 10);
    for (long long i = 0; i < n; i++) {
        if (i > 0) s += ',';
        s += genLiteral();
    }
    return s;
}
std::string goodStr() {
    std::string type  = genType();
    std::string name  = genName(1, NUMBER_OF_NAME);
    int count = randInt(1, NUMBER_OF_MAX_EL);
    return type + " " + name + "[" + std::to_string(count) + "]={" + genList(count) + "}";
}
std::string badStr() {
    int kind  = randInt(0, 16);
    std::string type  = genType();
    std::string name  = genName(1, 16);
    int count = randInt(1, 9);
    switch (kind) {
    case 0:
    {
        static const std::string bad[] = {"float", "double", "char", "INT", "Long"};
        return bad[randInt(0, 4)] + " " + name + "[" + std::to_string(count) + "]={" + genList(count) + "}";
    }
    case 1:
        return type + " " + std::to_string(randInt(0, 9)) + name
               + "[" + std::to_string(count) + "]={" + genList(count) + "}";
    case 2:
        return type + " " + genName(17, 10000)
               + "[" + std::to_string(count) + "]={" + genList(count) + "}";
    case 3:
        return type + " " + name + "[0]={" + genList(count) + "}";
    case 4:
        // исправлено: убран std::max, чтобы при count=1 список был пустым
        return type + " " + name + "[" + std::to_string(count) + "]={" + genList(count - 1) + "}";
    case 5:
        return type + " " + name + "[" + std::to_string(count) + "]={" + genList(count + 1) + "}";
    case 6:
        return type + " " + name + "[" + std::to_string(count) + "]={--1" + (count > 1 ? "," + genList(count - 1) : "") + "}";
    case 7:
        return type + " " + name + "[" + std::to_string(count) + "]=" + genList(count);
    case 8:
        // ведущий ноль в размере: [08], [007]
        return type + " " + name + "[0" + std::to_string(count) + "]={" + genList(count) + "}";
    case 9:
        // размер больше 9 цифр
        return type + " " + name + "[1234567890]={" + genList(count) + "}";
    case 10:
        // нет пробела между типом и именем
        return type + name + "[" + std::to_string(count) + "]={" + genList(count) + "}";
    case 11:
        // мусор после закрывающей фигурной
        return type + " " + name + "[" + std::to_string(count) + "]={" + genList(count) + "}xyz";
    case 12:
        // пустой список
        return type + " " + name + "[" + std::to_string(count) + "]={}";
    case 13:
        // подчёркивание в имени
        return type + " " + name + "_x[" + std::to_string(count) + "]={" + genList(count) + "}";
    case 14:
        // плюс перед литералом
        return type + " " + name + "[" + std::to_string(count) + "]={+1" + (count > 1 ? "," + genList(count - 1) : "") + "}";
    case 15:
        // литерал с точкой
        return type + " " + name + "[" + std::to_string(count) + "]={1.5" + (count > 1 ? "," + genList(count - 1) : "") + "}";
    case 16:
        // пробел внутри списка литералов
        return type + " " + name + "[" + std::to_string(count) + "]={ " + genList(count) + "}";
    default:
        return badStr();
    }
}
int main() {
    std::cout << "Generation mode:\n"
              << "  1 - only valid strings\n"
              << "  2 - valid and invalid strings\n"
              << "Mode: ";
    int mode; std::cin >> mode;
    std::string path = "/home//CLionProjects/strGenT/1.txt";
    std::ofstream out(path);
    if (!out) { std::cerr << "Failed to open file.\n"; return 1; }
    int good = 0, bad = 0;
    for (size_t i = 0; i < NUMBER_OF_STRINGS; i++) {
        if (mode == 1 || d(gen) == 1) { out << goodStr() << '\n'; ++good; }
        else                          { out << badStr()  << '\n'; ++bad;  }
    }
    out.close();
    std::cout << "Done: " << good << " valid, " << bad << " invalid.\n";
    return 0;
}