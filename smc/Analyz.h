#ifndef LAB1T_ANALYZ_H
#define LAB1T_ANALYZ_H
#include <set>
#include <unordered_map>
#include <string>
class Analyz {
public:
    bool isAcceptable=false;
    std::string arr;
    std::unordered_map<std::string, int> params;        // пара имя - число вхождений переменных
    std::unordered_map<std::string, std::string> types; // пара имя - тип переменной
    std::set<std::string> conflicts;
    virtual bool checkString(std::string& str)=0;
    virtual void checkFile(std::ifstream& file)=0;
    std::string getTable() {
        std::string t;
        for (const auto& [key,value]: params) {
            t+=key + "=" + std::to_string(value)+"\n";
        }
        return t;
    }

    virtual ~Analyz() = default;
};
#endif //LAB1T_ANALYZ_H
