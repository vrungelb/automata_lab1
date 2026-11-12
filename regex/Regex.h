#ifndef LAB1T_REGEX_H
#define LAB1T_REGEX_H

#include "../Analyz.h"
#include <algorithm>
#include <cctype>
#include <regex>
#include <stdexcept>

class Regex : public Analyz {
    std::regex fullPattern;

public:
    Regex()
        : fullPattern(
              R"((int|short|long) ([a-zA-Z][a-zA-Z0-9]{0,15})\[([1-9][0-9]{0,8})\]=\{(-?[0-9]+(?:,-?[0-9]+)*)\})",
              std::regex::optimize
          )
    {}

    bool checkString(std::string& str) override {
        isAcceptable = false;
        std::smatch match;
        if (std::regex_match(str, match, fullPattern)) {
            std::string currentType = match[1].str();
            std::string arrName     = match[2].str();

            std::transform(arrName.begin(), arrName.end(), arrName.begin(),
                [](unsigned char c) {return std::tolower(c);});

            long long declaredSize;
            try {
                declaredSize = std::stoll(match[3].str());
            } catch (const std::out_of_range&) {
                isAcceptable = false;
                return false;
            }
            long long actualCount = countElements(match[4].first, match[4].second);

            // семантическая проверка
            if (actualCount != declaredSize) {
                isAcceptable = false;
                return false;
            }

            recordValid(arrName, currentType);
            isAcceptable = true;
            return true;
        }
        return false;
    }

private:
    static long long countElements(std::string::const_iterator first, std::string::const_iterator last) {
        if (first == last) return 0;
        return 1 + std::count(first, last, ',');
    }
};

#endif