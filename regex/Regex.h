#ifndef LAB1T_REGEX_H
#define LAB1T_REGEX_H

#include "../smc/Analyz.h"
#include <regex>
#include <fstream>

class Regex : public Analyz {
    std::regex fullPattern;

public:
    Regex()
        : fullPattern(
              R"((int|short|long)\s([a-zA-Z][a-zA-Z0-9]{0,15})\[([1-9][0-9]{0,8})\]=\{(-?[0-9]+(?:,-?[0-9]+)*)\})",
              std::regex::optimize
          )
    {}

    bool checkString(std::string& str) override {
        isAcceptable = false;
        std::smatch match;
        if (std::regex_match(str, match, fullPattern)) {
            std::string currentType = match[1].str();
            std::string arrName     = match[2].str();
            std::string sizeStr     = match[3].str();
            std::string elements    = match[4].str();

            std::transform(arrName.begin(), arrName.end(), arrName.begin(),
                [](unsigned char c) {return std::tolower(c);});

            int declaredSize = std::stoi(sizeStr);
            int actualCount  = countElements(elements);

            // семантическая проверка
            if (actualCount != declaredSize) {
                isAcceptable = false;
                return false;
            }

            if (!types.contains(arrName)) {
                types[arrName] = currentType;
            } else {
                conflicts.insert(arrName + " - " + types[arrName]);
            }
            params[arrName]++;
            isAcceptable = true;
            return true;
        }
        return false;
    }

    void checkFile(std::ifstream& file) override {
        std::string line;
        while (std::getline(file, line)) {
            checkString(line);
        }
    }

private:
    static int countElements(const std::string& s) {
        if (s.find_first_not_of(" \t\n\r") == std::string::npos) return 0;
        int count = 1;
        for (char c : s) if (c == ',') count++;
        return count;
    }
};

#endif